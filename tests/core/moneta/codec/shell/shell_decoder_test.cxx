// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include <moneta/codec/shell/shell_decoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/Person.hxx"
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/Address.hxx"

//
// TODO: Review the need.
//
// static Composite make_composite() {
// 	Composite composite;
// 	composite.Identifier = 2600;
// 	composite.Person.ID = 123;
// 	composite.Person.Name = "Somedude";
// 	composite.Person.Height = 1.50;
// 	composite.Person.Ratings = 10;
// 	composite.Dog.Owner = "Someowner";
// 	composite.Dog.ID = 555;
// 	composite.Dog.Name = "Doggy";
// 	return composite;
// }

BOOST_AUTO_TEST_CASE(detail_special_split_test) {
	using moneta::codec::shell_decoder_implementation::special_split;
		
	{
		std::vector<std::string> v = special_split("{a=1 b=2 c=3}");
		BOOST_REQUIRE_EQUAL(v.size(), 3);
		BOOST_CHECK_EQUAL(v[0], "a=1");
		BOOST_CHECK_EQUAL(v[1], "b=2");
		BOOST_CHECK_EQUAL(v[2], "c=3");
	}

	{
		std::vector<std::string> v = special_split("{x={a=1 b=2 c=3}}");
		BOOST_REQUIRE_EQUAL(v.size(), 1);
		BOOST_CHECK_EQUAL(v[0], "x={a=1 b=2 c=3}");
	}

	{
		std::vector<std::string> v = special_split("{a=\"r s t\" b='t u v' c=\"{'a'}\"}");
		BOOST_REQUIRE_EQUAL(v.size(), 3);
		BOOST_CHECK_EQUAL(v[0], "a=r s t");
		BOOST_CHECK_EQUAL(v[1], "b=t u v");
		BOOST_CHECK_EQUAL(v[2], "c={'a'}");
	}

	{
		std::vector<std::string> v = special_split("{Dog={Name='Sam Doe'}}");
		BOOST_REQUIRE_EQUAL(v.size(), 1);
		BOOST_CHECK_EQUAL(v[0], "Dog={Name='Sam Doe'}");
	}
}

BOOST_AUTO_TEST_CASE(shell_codec_from_line_test) {
	using moneta::codec::shell_decoder_implementation::from_line;

	{
		const char* line = "{ID=1 Name=John Height=1.80 Ratings=10}";
		const Person person = from_line<Person>(line);
		BOOST_CHECK_EQUAL(person.ID, 1);
		BOOST_CHECK_EQUAL(person.Name, "John");
		BOOST_CHECK_EQUAL(person.Ratings, 10);
	}

	{
		const char* line = "{Owner=Charlie ID=150 Name=Snoopy}";
		const Dog dog = from_line<Dog>(line);
		BOOST_CHECK_EQUAL(dog.Owner, "Charlie");
		BOOST_CHECK_EQUAL(dog.ID, 150);
		BOOST_CHECK_EQUAL(dog.Name, "Snoopy");
	}

	{
		const char* line = "{Identifier=2600 Person={ID=5 Name=John Height=1.8 Ratings=12} Dog={Owner='Charlie Brown' ID=1 Name=Snoopy}}";
		const Composite composite = from_line<Composite>(line);
		BOOST_CHECK_EQUAL(composite.Identifier, 2600);

		BOOST_CHECK_EQUAL(composite.Person.ID, 5);
		BOOST_CHECK_EQUAL(composite.Person.Name, "John");
		BOOST_CHECK_CLOSE(composite.Person.Height, 1.80, 0.1);
		BOOST_CHECK_EQUAL(composite.Person.Ratings, 12);

		BOOST_CHECK_EQUAL(composite.Dog.Owner, "Charlie Brown");
		BOOST_CHECK_EQUAL(composite.Dog.ID, 1);
		BOOST_CHECK_EQUAL(composite.Dog.Name, "Snoopy");
	}
}

BOOST_AUTO_TEST_CASE(shell_decoder_test) {
	typedef moneta::codec::shell_decoder_implementation::shell_decoder<
		moneta::traits::entity_group<
			Person, Composite, Dog
		>
	>::type decoder_type;

	const std::string line = "Person={ID=5 Name=John Height=1.8 Ratings=12}";
	
	decoder_type::variant_type variant;
	const int result = decoder_type()(line.begin(), line.end(), variant);
	BOOST_REQUIRE_EQUAL(result, line.size());

	Person* person = boost::get<Person>(&variant);
	BOOST_REQUIRE(person);

	BOOST_CHECK_EQUAL(person->ID, 5);
	BOOST_CHECK_EQUAL(person->Name, "John");
	BOOST_CHECK_EQUAL(person->Height, 1.8);
	BOOST_CHECK_EQUAL(person->Ratings, 12);

	{
		const std::string line = "Person={ID=1 Name=John Height=1.80 Ratings=10}";

		decoder_type::variant_type variant;
		const int result = decoder_type()(line.begin(), line.end(), variant);
		BOOST_REQUIRE_EQUAL(result, line.size());

		Person* person = boost::get<Person>(&variant);
		BOOST_REQUIRE(person);

		BOOST_CHECK_EQUAL(person->ID, 1);
		BOOST_CHECK_EQUAL(person->Name, "John");
		BOOST_CHECK_EQUAL(person->Ratings, 10);
	}

	{
		const std::string line = "Dog={Owner=Charlie ID=150 Name=Snoopy}";

		decoder_type::variant_type variant;
		const int result = decoder_type()(line.begin(), line.end(), variant);
		BOOST_REQUIRE_EQUAL(result, line.size());

		Dog* dog = boost::get<Dog>(&variant);
		BOOST_REQUIRE(dog);

		BOOST_CHECK_EQUAL(dog->Owner, "Charlie");
		BOOST_CHECK_EQUAL(dog->ID, 150);
		BOOST_CHECK_EQUAL(dog->Name, "Snoopy");
	}

	{
		const std::string line = "Composite={Identifier=2600 Person={ID=5 Name=John Height=1.8 Ratings=12} Dog={Owner='Charlie Brown' ID=1 Name=Snoopy}}";

		decoder_type::variant_type variant;
		const int result = decoder_type()(line.begin(), line.end(), variant);
		BOOST_REQUIRE_EQUAL(result, line.size());

		Composite* composite = boost::get<Composite>(&variant);
		BOOST_CHECK_EQUAL(composite->Identifier, 2600);

		BOOST_CHECK_EQUAL(composite->Person.ID, 5);
		BOOST_CHECK_EQUAL(composite->Person.Name, "John");
		BOOST_CHECK_CLOSE(composite->Person.Height, 1.80, 0.1);
		BOOST_CHECK_EQUAL(composite->Person.Ratings, 12);

		BOOST_CHECK_EQUAL(composite->Dog.Owner, "Charlie Brown");
		BOOST_CHECK_EQUAL(composite->Dog.ID, 1);
		BOOST_CHECK_EQUAL(composite->Dog.Name, "Snoopy");
	}
}
