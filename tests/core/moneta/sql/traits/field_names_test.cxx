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
#include <moneta/sql/traits/field_names.hxx>
#include "../../model/Person.hxx"

BOOST_AUTO_TEST_CASE(field_name_test) {
	BOOST_CHECK_EQUAL(
		(moneta::traits::sql_field_name<MONETA_MEMBER(Person, int, ID)>::get()),
		"PERSON_ID"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::sql_field_name<MONETA_MEMBER(Person, std::string, Name)>::get()),
		"PERSON_NAME"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::sql_field_name<MONETA_MEMBER(Person, double, Height)>::get()),
		"PERSON_HEIGHT"
	);

	BOOST_CHECK_EQUAL(
		(moneta::traits::sql_field_name<MONETA_MEMBER(Person, int, Ratings)>::get()),
		"PERSON_RATINGS"
	);
}

BOOST_AUTO_TEST_CASE(field_names_test) {
	std::vector<std::string> names = moneta::sql::traits::get_field_names<Person>();
	BOOST_REQUIRE_EQUAL(names.size(), 4);
	BOOST_CHECK_EQUAL(names[0], "PERSON_ID");
	BOOST_CHECK_EQUAL(names[1], "PERSON_NAME");
	BOOST_CHECK_EQUAL(names[2], "PERSON_HEIGHT");
	BOOST_CHECK_EQUAL(names[3], "PERSON_RATINGS");
}
