#include "stdafx.h"
#include <moneta/serialization/shell/shell_codec.hxx>
#include <boost/test/floating_point_comparison.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"

BOOST_AUTO_TEST_CASE(shell_codec_test) {
	using moneta::serialization::shell::from_line;
	using moneta::serialization::shell::to_line;

	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Fingers = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";

	const std::string line = to_line(composite);
	BOOST_CHECK_EQUAL(line, "{Identifier=2600, Person={ID=123, Name=Somedude, Height=1.5, Fingers=10}, Dog={Owner=Someowner, ID=555, Name=Doggy}}");

	//Composite phoenix = moneta::serialization::shell::from_line<Composite>(line);
	//BOOST_CHECK_EQUAL(person.Name, "John");

	const Person person = from_line<Person>("ID=1 Name=John Height=1.80 Fingers=10");
	BOOST_CHECK_EQUAL(person.ID, 1);
	BOOST_CHECK_EQUAL(person.Name, "John");
	BOOST_CHECK_EQUAL(person.Fingers, 10);
	std::cerr << to_line(person) << std::endl;
}
