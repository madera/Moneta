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
#include <moneta/make_entity.hxx>
#include "model/Person.hxx"
#include "model/Dog.hxx"
#include "model/Composite.hxx"
#include "model/simple/FixedThreeInts.hxx"

BOOST_AUTO_TEST_CASE(make_entity_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.ID, 0);
	BOOST_CHECK(person.Name.empty());
	BOOST_CHECK_EQUAL(person.Height, 0.00);
	BOOST_CHECK_EQUAL(person.Ratings, 0);

	Dog dog = moneta::make_entity<Dog>();
	BOOST_CHECK(dog.Owner.empty());
	BOOST_CHECK_EQUAL(dog.ID, 0);
	BOOST_CHECK(dog.Name.empty());

	Composite composite = moneta::make_entity<Composite>();
	BOOST_CHECK_EQUAL(composite.Identifier, 0);

	BOOST_CHECK_EQUAL(composite.Person.ID, 0);
	BOOST_CHECK(composite.Person.Name.empty());
	BOOST_CHECK_EQUAL(composite.Person.Height, 0.00);
	BOOST_CHECK_EQUAL(composite.Person.Ratings, 0);

	BOOST_CHECK(composite.Dog.Owner.empty());
	BOOST_CHECK_EQUAL(composite.Dog.ID, 0);
	BOOST_CHECK(composite.Dog.Name.empty());
}

BOOST_AUTO_TEST_CASE(fixed_values_make_entity_test) {
	FixedThreeInts entity = moneta::make_entity<FixedThreeInts>();
	BOOST_CHECK_EQUAL(entity.One, 0x11);
	BOOST_CHECK_EQUAL(entity.Two, 0x22);
	BOOST_CHECK_EQUAL(entity.Three, 0);
}

BOOST_AUTO_TEST_CASE(const_make_entity_test) {
	FixedThreeInts entity = moneta::make_entity<const FixedThreeInts>();
	BOOST_CHECK_EQUAL(entity.One, 0x11);
	BOOST_CHECK_EQUAL(entity.Two, 0x22);
	BOOST_CHECK_EQUAL(entity.Three, 0);
}
