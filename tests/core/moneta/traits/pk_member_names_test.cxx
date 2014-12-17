#include "stdafx.h"
#include <moneta/traits/pk_member_names.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(pk_member_names_test) {
	{
		std::vector<std::string> names = moneta::traits::get_pk_member_names<Person>();
		BOOST_REQUIRE_EQUAL(names.size(), 1);
		BOOST_CHECK_EQUAL(names[0], "ID");
	}

	{
		std::vector<std::string> names = moneta::traits::get_pk_member_names<Dog>();
		BOOST_REQUIRE_EQUAL(names.size(), 2);
		BOOST_CHECK_EQUAL(names[0], "Owner");
		BOOST_CHECK_EQUAL(names[1], "ID");
	}
}
