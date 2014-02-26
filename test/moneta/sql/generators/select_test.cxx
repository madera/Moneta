#include "stdafx.h"
#include <moneta/sql/generators/select.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(select_all_from_table_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table<Person>(),
		"SELECT ID, NAME, HEIGHT, FINGERS\n  FROM PERSON"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table<Dog>(),
		"SELECT OWNER, ID, NAME\n  FROM DOG"
	);
}

BOOST_AUTO_TEST_CASE(select_all_from_table_where_pk_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table_where_pk<Person>(),
		"SELECT ID, NAME, HEIGHT, FINGERS\n  FROM PERSON\n WHERE ID = :id"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table_where_pk<Dog>(),
		"SELECT OWNER, ID, NAME\n  FROM DOG\n WHERE Owner = :owner AND ID = :id"
	);
}
