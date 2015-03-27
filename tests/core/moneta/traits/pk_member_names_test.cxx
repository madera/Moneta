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
#include <moneta/traits/pk_member_names.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(pk_member_names_test) {
	{
		std::vector<std::string> names = moneta::traits::get_pk_member_names<Person>();
		BOOST_REQUIRE_EQUAL(names.size(), 1);
		BOOST_CHECK_EQUAL(names[0], "ID");
	}

	{
		std::vector<std::string> names = moneta::traits::get_pk_member_names<Dog>();
		BOOST_REQUIRE_EQUAL(names.size(), 2);
		BOOST_CHECK_EQUAL(names[0], "Owner");
		BOOST_CHECK_EQUAL(names[1], "ID");
	}
}
