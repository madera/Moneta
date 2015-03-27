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

#include "stdafx.h"
#include "detail/soci_fixture.hxx"
#include <moneta/serialization/soci/soci_count.hxx>
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(manual_soci_count_test, soci_test_fixture) {
	size_t count = 0;
	soci_session << "select count(*) from CAT", ::soci::into(count);
	BOOST_CHECK_EQUAL(count, 7);
}

BOOST_FIXTURE_TEST_CASE(soci_count_test, soci_test_fixture) {
	BOOST_CHECK_EQUAL(moneta::serialization::soci::soci_count<Person>(soci_session), 10);
	BOOST_CHECK_EQUAL(moneta::serialization::soci::soci_count<Cat>(soci_session), 7);
}
