#include "stdafx.h"
#include <moneta/traits/member_names.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Address.hxx"

BOOST_AUTO_TEST_CASE(member_name_test) {
	//
	// Person
	//
	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Person, int, ID)>::get()),
		"ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Person, std::string, Name)>::get()),
		"Name"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Person, double, Height)>::get()),
		"Height"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Person, int, Fingers)>::get()),
		"Fingers"
	);

	//
	// Dog
	//
	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Dog, std::string, Owner)>::get()),
		"Owner"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Dog, int, ID)>::get()),
		"ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Dog, std::string, Name)>::get()),
		"Name"
	);

	//
	// Address
	//
	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Address, int, ID)>::get()),
		"ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Address, int, Number)>::get()),
		"Number"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::detail::member_name<MONETA_MEMBER(Address, std::string, Street)>::get()),
		"Street"
	);
}

BOOST_AUTO_TEST_CASE(member_names_test) {
	{
		std::vector<std::string> names = moneta::traits::get_member_names<Person>();
		BOOST_REQUIRE_EQUAL(names.size(), 4);
		BOOST_CHECK_EQUAL(names[0], "ID");
		BOOST_CHECK_EQUAL(names[1], "Name");
		BOOST_CHECK_EQUAL(names[2], "Height");
		BOOST_CHECK_EQUAL(names[3], "Fingers");
	}

	{
		std::vector<std::string> names = moneta::traits::get_member_names<Dog>();
		BOOST_REQUIRE_EQUAL(names.size(), 3);
		BOOST_CHECK_EQUAL(names[0], "Owner");
		BOOST_CHECK_EQUAL(names[1], "ID");
		BOOST_CHECK_EQUAL(names[2], "Name");
	}
}

BOOST_AUTO_TEST_CASE(get_member_name_index_test) {
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("ID"), 0);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("Name"), 1);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("Height"), 2);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("Fingers"), 3);

	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Dog>("Owner"), 0);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Dog>("ID"), 1);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Dog>("Name"), 2);
}

BOOST_AUTO_TEST_CASE(get_member_name_test) {
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Person>(0), "ID");
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Person>(1), "Name");
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Person>(2), "Height");
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Person>(3), "Fingers");

	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Dog>(0), "Owner");
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Dog>(1), "ID");
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name<Dog>(2), "Name");
}
