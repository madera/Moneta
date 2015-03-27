#include "pch.hxx"
#include <moneta/sql/traits/table_name.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(table_name_test) {
	BOOST_CHECK_EQUAL(moneta::sql::traits::get_table_name<Person>(), "PERSON");

	BOOST_CHECK_EQUAL(moneta::sql::traits::get_table_name<Dog>(), "DOG");
}
