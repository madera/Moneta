#include "stdafx.h"
#include <moneta/make_entity.hxx>
#include <moneta/lexical/set_value.hxx>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(set_value_by_name_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.Name, "");
	BOOST_CHECK_EQUAL(person.Height, 0.0);
	BOOST_CHECK_EQUAL(person.Fingers, 0);

	moneta::lexical::set_value(person, "ID", "123");
	BOOST_CHECK_EQUAL(person.ID, 123);
	
	moneta::lexical::set_value(person, "Name", "Somebody");
	BOOST_CHECK_EQUAL(person.Name, "Somebody");

	moneta::lexical::set_value(person, "Height", "1.80");
	BOOST_CHECK_CLOSE(person.Height, 1.80, 0.1);
	
	moneta::lexical::set_value(person, "Fingers", "100");
	BOOST_CHECK_EQUAL(person.Fingers, 100);
}

BOOST_AUTO_TEST_CASE(set_value_by_ordinal_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.Name, "");
	BOOST_CHECK_EQUAL(person.Fingers, 0);
	BOOST_CHECK_EQUAL(person.Height, 0.0);

	moneta::lexical::set_value(person, 0, "123");
	BOOST_CHECK_EQUAL(person.ID, 123);
	
	moneta::lexical::set_value(person, 1, "Somebody");
	BOOST_CHECK_EQUAL(person.Name, "Somebody");
	
	moneta::lexical::set_value(person, 2, "1.80");
	BOOST_CHECK_CLOSE(person.Height, 1.80, 0.1);

	moneta::lexical::set_value(person, 3, "100");
	BOOST_CHECK_EQUAL(person.Fingers, 100);
}

BOOST_AUTO_TEST_CASE(set_value_by_member_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.Name, "");
	BOOST_CHECK_EQUAL(person.Fingers, 0);
	BOOST_CHECK_EQUAL(person.Height, 0.0);

	moneta::lexical::set_value<MONETA_MEMBER(Person, std::string, Name)>(person, "Charlie Brown");
	BOOST_CHECK_EQUAL(person.Name, "Charlie Brown");

	moneta::lexical::set_value<MONETA_MEMBER(Person, double, Height)>(person, "1.80");
	BOOST_CHECK_EQUAL(person.Height, 1.80);

	moneta::lexical::set_value<MONETA_MEMBER(Person, int, Fingers)>(person, "10");
	BOOST_CHECK_EQUAL(person.Fingers, 10);
}
