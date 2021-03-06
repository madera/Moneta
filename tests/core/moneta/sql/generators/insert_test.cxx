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
#include <moneta/sql/generators/insert.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(insert_into_table_test) {
	BOOST_CHECK_EQUAL(
		moneta::sql::generators::insert_into_table<Person>(),
		"INSERT INTO PERSON (\n\tPERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_RATINGS\n) VALUES (\n\t:person_id, :person_name, :person_height, :person_ratings\n)"
	);

	BOOST_CHECK_EQUAL(
		moneta::sql::generators::insert_into_table<Dog>(),
		"INSERT INTO DOG (\n\tDOG_OWNER, DOG_ID, DOG_NAME\n) VALUES (\n\t:dog_owner, :dog_id, :dog_name\n)"
	);
}
