#include "pch.hxx"
#include <moneta/sql/generators/select.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(select_all_from_table_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table<Person>(),
		"SELECT PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_RATINGS\n  FROM PERSON"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table<Dog>(),
		"SELECT DOG_OWNER, DOG_ID, DOG_NAME\n  FROM DOG"
	);
}

BOOST_AUTO_TEST_CASE(select_all_from_table_where_pk_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table_where_pk<Person>(),
		"SELECT PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_RATINGS\n  FROM PERSON\n WHERE PERSON_ID = :person_id"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::select_all_from_table_where_pk<Dog>(),
		"SELECT DOG_OWNER, DOG_ID, DOG_NAME\n  FROM DOG\n WHERE DOG_OWNER = :dog_owner AND DOG_ID = :dog_id"
	);
}
