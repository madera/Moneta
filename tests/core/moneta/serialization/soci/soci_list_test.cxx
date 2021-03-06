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
#include <moneta/serialization/soci/soci_list.hxx>
#include "../../model/Person.hxx"
#include "../../model/Address.hxx"
#include "../../model/Cat.hxx"
#include <vector>

BOOST_FIXTURE_TEST_CASE(soci_list_test, soci_test_fixture) {
	std::vector<Address> addresses;
	const size_t count = moneta::serialization::soci::soci_list<Address>(soci_session,
		std::back_inserter<>(addresses));

	BOOST_CHECK_EQUAL(count, 3);
	BOOST_CHECK_EQUAL(addresses.size(), 3);
}
