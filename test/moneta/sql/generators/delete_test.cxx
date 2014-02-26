#include "stdafx.h"
#include <moneta/sql/generators/delete.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(delete_from_table_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table<Person>(),
		"DELETE FROM PERSON"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table<Dog>(),
		"DELETE FROM DOG"
	);
}

BOOST_AUTO_TEST_CASE(delete_from_table_where_pk_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table_where_pk<Person>(),
		"DELETE FROM PERSON\n WHERE ID = :id"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table_where_pk<Dog>(),
		"DELETE FROM DOG\n WHERE Owner = :owner AND ID = :id"
	);
}
