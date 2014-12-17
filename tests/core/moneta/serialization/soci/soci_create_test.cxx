#include "stdafx.h"
#include "detail/soci_fixture.hxx"
#include <moneta/serialization/soci/soci_create.hxx>
#include <moneta/serialization/soci/soci_read.hxx>
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(soci_create_test, soci_test_fixture) {
	Address address;
	address.ID = 555;
	address.Number = 123;
	address.Street = "Some Road";

	moneta::serialization::soci::soci_create<Address>(soci_session, address);

	Address entity = moneta::serialization::soci::soci_read<Address>(soci_session, 555);
	BOOST_CHECK_EQUAL(entity.ID, address.ID);
	BOOST_CHECK_EQUAL(entity.Number, address.Number);
	BOOST_CHECK_EQUAL(entity.Street, address.Street);
}


BOOST_FIXTURE_TEST_CASE(soci_create_recursive_test, soci_test_fixture) {
	Cat cat;
	cat.ID = 5550;
	cat.Name = "Some Cat";
	cat.Address.ID = 555;
	cat.Address.Number = 123;
	cat.Address.Street = "Some Road";

	moneta::serialization::soci::soci_create<Cat>(soci_session, cat);

	Cat entity = moneta::serialization::soci::soci_read<Cat>(soci_session, 5550);
	BOOST_CHECK_EQUAL(entity.ID, cat.ID);
	BOOST_CHECK_EQUAL(entity.Name, cat.Name);
	BOOST_CHECK_EQUAL(entity.Address.ID, cat.Address.ID);
	BOOST_CHECK_EQUAL(entity.Address.Number, cat.Address.Number);
	BOOST_CHECK_EQUAL(entity.Address.Street, cat.Address.Street);
}
