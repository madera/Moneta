#include "stdafx.h"
#include <moneta/serialization/iostream.hxx>

#include "../model/Person.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(serialization_roundtrip_iostream_test) {
	Person person;
	person.ID = 345;
	person.Name = "Anonymous";
	person.Height = 1.75;
	person.Fingers = 12;

	const std::string textual = boost::lexical_cast<std::string>(person);
	BOOST_CHECK_EQUAL(textual, "{ID=345 Name=Anonymous Height=1.75 Fingers=12}");

	Person phoenix = boost::lexical_cast<Person>(textual);
	const std::string phoenix_string = boost::lexical_cast<std::string>(phoenix);
	BOOST_CHECK_EQUAL(textual, phoenix_string);
}

// TODO: Test other formats...
// TODO: Rethink IO strategy... maybe separate IO implementations.
