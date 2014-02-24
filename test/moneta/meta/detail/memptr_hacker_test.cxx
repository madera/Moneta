#include "stdafx.h"
#include <moneta/meta/detail/memptr_hacker.hxx>
#include "../../model/Person.hxx"

BOOST_AUTO_TEST_CASE(memptr_hacker_member_ordinal_test) {
	BOOST_REQUIRE_EQUAL(moneta::member_ordinal(&Person::ID     ), 0);
	BOOST_REQUIRE_EQUAL(moneta::member_ordinal(&Person::Name   ), 1);
	BOOST_REQUIRE_EQUAL(moneta::member_ordinal(&Person::Height ), 2);
	BOOST_REQUIRE_EQUAL(moneta::member_ordinal(&Person::Fingers), 3);
}
