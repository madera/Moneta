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
#include <moneta/sql/generators/delete.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(delete_from_table_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table<Person>(),
		"DELETE FROM PERSON"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table<Dog>(),
		"DELETE FROM DOG"
	);
}

BOOST_AUTO_TEST_CASE(delete_from_table_where_pk_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table_where_pk<Person>(),
		"DELETE FROM PERSON\n WHERE PERSON_ID = :person_id"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::delete_from_table_where_pk<Dog>(),
		"DELETE FROM DOG\n WHERE DOG_OWNER = :dog_owner AND DOG_ID = :dog_id"
	);
}
