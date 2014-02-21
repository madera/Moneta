#include "stdafx.h"
#include <moneta/reflection/detail/memptr_hacker.hxx>

struct k0 {
	int a;
	int b;
	int c;
	std::string d;
	std::string e;
	std::string f;
};

BOOST_AUTO_TEST_CASE(memptr_hacker_member_ordinal_test) {
	//BOOST_REQUIRE_EQUAL(moneta::detail::get_memptr_fingerprint<>(&k0::a), 0);
	//BOOST_REQUIRE_EQUAL(moneta::detail::get_memptr_fingerprint<>(&k0::b), 1);
	//BOOST_REQUIRE_EQUAL(moneta::detail::get_memptr_fingerprint<>(&k0::c), 2);
	//BOOST_REQUIRE_EQUAL(moneta::detail::get_memptr_fingerprint<>(&k0::d), 3);
	//BOOST_REQUIRE_EQUAL(moneta::detail::get_memptr_fingerprint<>(&k0::e), 4);
	//BOOST_REQUIRE_EQUAL(moneta::detail::get_memptr_fingerprint<>(&k0::f), 5);
}
