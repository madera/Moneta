#include "stdafx.h"
#include <moneta/meta/member_names.hxx>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(member_names_test) {
	std::vector<std::string> names = moneta::meta::get_member_names<Person>();
	BOOST_REQUIRE_EQUAL(names.size(), 4);
	BOOST_CHECK_EQUAL(names[0], "ID");
	BOOST_CHECK_EQUAL(names[1], "Name");
	BOOST_CHECK_EQUAL(names[2], "Height");
	BOOST_CHECK_EQUAL(names[3], "Fingers");
}
