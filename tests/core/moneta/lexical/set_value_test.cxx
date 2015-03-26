#include "stdafx.h"
#include <moneta/make_entity.hxx>
#include <moneta/lexical/set_value.hxx>
#include "../model/Person.hxx"
#include "../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(set_value_by_name_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.Name, "");
	BOOST_CHECK_EQUAL(person.Height, 0.0);
	BOOST_CHECK_EQUAL(person.Ratings, 0);

	moneta::lexical::set_value(person, "ID", "123");
	BOOST_CHECK_EQUAL(person.ID, 123);
	
	moneta::lexical::set_value(person, "Name", "Somebody");
	BOOST_CHECK_EQUAL(person.Name, "Somebody");

	moneta::lexical::set_value(person, "Height", "1.80");
	BOOST_CHECK_CLOSE(person.Height, 1.80, 0.1);
	
	moneta::lexical::set_value(person, "Ratings", "100");
	BOOST_CHECK_EQUAL(person.Ratings, 100);
}

BOOST_AUTO_TEST_CASE(illegal_set_value_test) {
	Cat cat = moneta::make_entity<Cat>();

	moneta::lexical::set_value(cat, "ID", "123");
	BOOST_CHECK_EQUAL(cat.ID, 123);

	moneta::lexical::set_value(cat, "Name", "Garfield");
	BOOST_CHECK_EQUAL(cat.Name, "Garfield");

	/* Test set_value() on an Entity member */ {
		int id = cat.Address.ID;
		int number = cat.Address.Number;
		std::string street = cat.Address.Street;

		moneta::lexical::set_value(cat, "Address", "Ignored");

		BOOST_CHECK_EQUAL(cat.Address.ID, id);
		BOOST_CHECK_EQUAL(cat.Address.Number, number);
		BOOST_CHECK_EQUAL(cat.Address.Street, street);
	}
}
