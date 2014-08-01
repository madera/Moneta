#include "stdafx.h"
#include <moneta/codec/shell/shell_encoder.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"

#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/serialization/iostream.hxx>

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

BOOST_AUTO_TEST_CASE(XXXshell_codec_to_line_test) {
	const Composite composite = make_composite();
	const std::string line = moneta::serialization::shell::to_line(composite);
	
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	moneta::codec::encode<moneta::codec::shell>(composite, buffer, buffer + sizeof(buffer));

	//BOOST_CHECK_EQUAL(line, "{Identifier=2600 Person={ID=123 Name=Somedude Height=1.5 Fingers=10} Dog={Owner=Someowner ID=555 Name=Doggy}}");
}

BOOST_AUTO_TEST_CASE(XXXshell_codec_from_line_test) {
	{
		const char* line = "{ID=1 Name=John Height=1.80 Fingers=10}";
		const Person person = moneta::serialization::shell::from_line<Person>(line);
		BOOST_CHECK_EQUAL(person.ID, 1);
		BOOST_CHECK_EQUAL(person.Name, "John");
		BOOST_CHECK_EQUAL(person.Fingers, 10);
	}

	{
		const char* line = "{Owner=Charlie ID=150 Name=Snoopy}";
		const Dog dog = moneta::serialization::shell::from_line<Dog>(line);
		BOOST_CHECK_EQUAL(dog.Owner, "Charlie");
		BOOST_CHECK_EQUAL(dog.ID, 150);
		BOOST_CHECK_EQUAL(dog.Name, "Snoopy");
	}

	{
		const char* line = "{Identifier=2600 Person={ID=5 Name=John Height=1.8 Fingers=12} Dog={Owner='Charlie Brown' ID=1 Name=Snoopy}}";
		const Composite composite = moneta::serialization::shell::from_line<Composite>(line);
		BOOST_CHECK_EQUAL(composite.Identifier, 2600);

		BOOST_CHECK_EQUAL(composite.Person.ID, 5);
		BOOST_CHECK_EQUAL(composite.Person.Name, "John");
		BOOST_CHECK_CLOSE(composite.Person.Height, 1.80, 0.1);
		BOOST_CHECK_EQUAL(composite.Person.Fingers, 12);

		BOOST_CHECK_EQUAL(composite.Dog.Owner, "Charlie Brown");
		BOOST_CHECK_EQUAL(composite.Dog.ID, 1);
		BOOST_CHECK_EQUAL(composite.Dog.Name, "Snoopy");

		// Roundtrip
		const std::string serialized = moneta::serialization::shell::to_line(composite);
		BOOST_CHECK_EQUAL(serialized, line);
	}
}
