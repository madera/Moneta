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
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(to_tie_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Ratings = 10;
	
	moneta::traits::tie<Person>::type tie = moneta::traits::to_tie(person);
	boost::fusion::at_c<0>(tie) = 10;
	boost::fusion::at_c<3>(tie)++;

	// Verify that changes were made to the instance.
	BOOST_CHECK_EQUAL(person.ID, 10);
	BOOST_CHECK_EQUAL(person.Ratings, 11);
}

BOOST_AUTO_TEST_CASE(const_to_tie_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Ratings = 10;
	
	moneta::traits::tie<const Person>::type const_tie = moneta::traits::to_tie(person);
	// TODO: Test value.

	const Person& const_person = person;
	moneta::traits::tie<const Person>::type const_tie2 = moneta::traits::to_tie(const_person);
	// TODO: Test value.
}
