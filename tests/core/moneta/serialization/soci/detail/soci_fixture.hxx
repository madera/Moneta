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

#pragma once
#include <moneta/serialization/soci/detail/soci_includes.hxx>

void create_schema(soci::session& session);
void insert_data(soci::session& session);

struct soci_test_fixture {
	soci::session soci_session;

	soci_test_fixture()
	 : soci_session(soci::sqlite3, ":memory:") {
		create_schema(soci_session);
		insert_data(soci_session);
	}

	~soci_test_fixture() {
	}
};
