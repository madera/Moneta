#include "stdafx.h"
#include "detail/soci_fixture.hxx"
#include <moneta/serialization/soci/soci_read.hxx>
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(soci_read_test, soci_test_fixture) {
	//Person person = moneta::serialization::soci::soci_read<Person>(soci_session, 1);
	Address address = moneta::serialization::soci::soci_read<Address>(soci_session, 1);
	//Cat cat = moneta::serialization::soci::soci_read<Cat>(soci_session, 1);
}
