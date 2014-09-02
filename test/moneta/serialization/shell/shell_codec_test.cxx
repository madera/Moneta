#include "stdafx.h"
#include <moneta/serialization/shell/shell_codec.hxx>
#include <boost/test/floating_point_comparison.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"

static Composite make_composite() {
	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Fingers = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";
	return composite;
}

BOOST_AUTO_TEST_CASE(detail_special_split_test) {
	using moneta::serialization::shell::detail::special_split;
		
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
	{
		const char* line = "{ID=1 Name=John Height=1.80 Fingers=10}";
		const Person person = moneta::serialization::shell::from_line<Person>(line);
		BOOST_CHECK_EQUAL(person.ID, 1);
		BOOST_CHECK_EQUAL(person.Name, "John");
		BOOST_CHECK_EQUAL(person.Fingers, 10);
	}

	//{
	//	const char* line = "{Owner=Charlie ID=150 Name=Snoopy}";
	//	const Dog dog = moneta::serialization::shell::from_line<Dog>(line);
	//	BOOST_CHECK_EQUAL(dog.Owner, "Charlie");
	//	BOOST_CHECK_EQUAL(dog.ID, 150);
	//	BOOST_CHECK_EQUAL(dog.Name, "Snoopy");
	//}

	//{
	//	const char* line = "{Identifier=2600 Person={ID=5 Name=John Height=1.8 Fingers=12} Dog={Owner='Charlie Brown' ID=1 Name=Snoopy}}";
	//	const Composite composite = moneta::serialization::shell::from_line<Composite>(line);
	//	BOOST_CHECK_EQUAL(composite.Identifier, 2600);

	//	BOOST_CHECK_EQUAL(composite.Person.ID, 5);
	//	BOOST_CHECK_EQUAL(composite.Person.Name, "John");
	//	BOOST_CHECK_CLOSE(composite.Person.Height, 1.80, 0.1);
	//	BOOST_CHECK_EQUAL(composite.Person.Fingers, 12);

	//	BOOST_CHECK_EQUAL(composite.Dog.Owner, "Charlie Brown");
	//	BOOST_CHECK_EQUAL(composite.Dog.ID, 1);
	//	BOOST_CHECK_EQUAL(composite.Dog.Name, "Snoopy");
	//}
}
