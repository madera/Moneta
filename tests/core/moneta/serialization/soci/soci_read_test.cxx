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
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_FIXTURE_TEST_CASE(soci_read_test, soci_test_fixture) {
	Address address = moneta::serialization::soci::soci_read<Address>(soci_session, 1);
}

// TODO: Create recursive read test.
