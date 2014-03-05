#include "stdafx.h"
#include "detail/soci_fixture.hxx"
#include <moneta/serialization/soci/soci_count.hxx>
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(manual_soci_count_test, soci_test_fixture) {
	size_t count = 0;
	soci_session << "select count(*) from CAT", ::soci::into(count);
	BOOST_CHECK_EQUAL(count, 7);
}

BOOST_FIXTURE_TEST_CASE(soci_count_test, soci_test_fixture) {
	BOOST_CHECK_EQUAL(moneta::serialization::soci::soci_count<Cat>(soci_session), 7);
}
