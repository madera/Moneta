#include "stdafx.h"
#include "detail/soci_fixture.hxx"
#include <moneta/serialization/soci/soci_read.hxx>
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(soci_read_test, soci_test_fixture) {
	Address address = moneta::serialization::soci::soci_read<Address>(soci_session, 1);
}

// TODO: Create recursive read test.
