// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include <moneta/traits/member_name.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Address.hxx"

BOOST_AUTO_TEST_CASE(test_moneta_traits_member_names_get) {
	//
	// Person
	//
	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Person, int, ID)>::get()),
		"ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Person, std::string, Name)>::get()),
		"Name"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Person, double, Height)>::get()),
		"Height"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Person, int, Ratings)>::get()),
		"Ratings"
	);

	//
	// Dog
	//
	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Dog, std::string, Owner)>::get()),
		"Owner"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Dog, int, ID)>::get()),
		"ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Dog, std::string, Name)>::get()),
		"Name"
	);

	//
	// Address
	//
	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Address, int, ID)>::get()),
		"ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Address, int, Number)>::get()),
		"Number"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::member_name<MONETA_MEMBER(Address, std::string, Street)>::get()),
		"Street"
	);
}

BOOST_AUTO_TEST_CASE(test_moneta_traits_member_names_get_member_names) {
	{
		std::vector<std::string> names = moneta::traits::get_member_names<Person>();
		BOOST_REQUIRE_EQUAL(names.size(), 4);
		BOOST_CHECK_EQUAL(names[0], "ID");
		BOOST_CHECK_EQUAL(names[1], "Name");
		BOOST_CHECK_EQUAL(names[2], "Height");
		BOOST_CHECK_EQUAL(names[3], "Ratings");
	}

	{
		std::vector<std::string> names = moneta::traits::get_member_names<Dog>();
		BOOST_REQUIRE_EQUAL(names.size(), 3);
		BOOST_CHECK_EQUAL(names[0], "Owner");
		BOOST_CHECK_EQUAL(names[1], "ID");
		BOOST_CHECK_EQUAL(names[2], "Name");
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_traits_member_names_test_get_member_name_index) {
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("ID"), 0);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("Name"), 1);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("Height"), 2);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Person>("Ratings"), 3);

	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Dog>("Owner"), 0);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Dog>("ID"), 1);
	BOOST_CHECK_EQUAL(moneta::traits::get_member_name_index<Dog>("Name"), 2);
}
