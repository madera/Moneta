#include "stdafx.h"
#include <moneta/sql/generators/insert.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(insert_into_table_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::insert_into_table<Person>(),
		"INSERT INTO PERSON (\n\tID, NAME, HEIGHT, FINGERS\n) VALUES (\n\t:a, :b, :c, :d\n)"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::insert_into_table<Dog>(),
		"INSERT INTO DOG (\n\tOWNER, ID, NAME\n) VALUES (\n\t:a, :b, :c\n)"
	);
}
