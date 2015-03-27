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
#include <moneta/traits/to_tie.hxx>
#include <moneta/traits/to_entity.hxx>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(to_entity_test) {

	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Ratings = 10;

	moneta::traits::tie<Person>::type tie = moneta::traits::to_tie(person);

	Person copy = moneta::traits::to_entity<Person>(tie);

	BOOST_CHECK_EQUAL(person.ID, copy.ID);
	BOOST_CHECK_EQUAL(person.Name, copy.Name);
	BOOST_CHECK_EQUAL(person.Height, copy.Height);
	BOOST_CHECK_EQUAL(person.Ratings, copy.Ratings);
}
