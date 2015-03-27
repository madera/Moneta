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
#include <moneta/serialization/soci/soci_read.hxx>
#include <moneta/serialization/soci/soci_delete.hxx>
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(soci_delete_test, soci_test_fixture) {
	Address entity = moneta::serialization::soci::soci_read<Address>(soci_session, 1);
	BOOST_CHECK_EQUAL(entity.ID, 1);

	const size_t count = moneta::serialization::soci::soci_delete<Address>(soci_session, 1);
	BOOST_CHECK_EQUAL(count, 1);

	Address after = moneta::serialization::soci::soci_read<Address>(soci_session, 1);
	BOOST_CHECK_EQUAL(after.ID, 0);
	// XXX: This should be an empty_pk check!!!
}
