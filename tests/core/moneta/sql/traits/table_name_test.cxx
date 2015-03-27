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
#include <moneta/sql/traits/table_name.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(table_name_test) {
	BOOST_CHECK_EQUAL(moneta::sql::traits::get_table_name<Person>(), "PERSON");

	BOOST_CHECK_EQUAL(moneta::sql::traits::get_table_name<Dog>(), "DOG");
}
