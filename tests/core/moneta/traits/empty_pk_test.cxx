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
#include <moneta/traits/empty_pk.hxx>
#include <moneta/traits/extract_pk.hxx>
#include <moneta/make_entity.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(empty_pk_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK(moneta::traits::extract_pk(person) == moneta::traits::empty_pk<Person>());

	person.ID = 555;
	BOOST_CHECK(moneta::traits::extract_pk(person) != moneta::traits::empty_pk<Person>());


	Dog dog = moneta::make_entity<Dog>();
	BOOST_CHECK(moneta::traits::extract_pk(dog) == moneta::traits::empty_pk<Dog>());

	dog.Owner = "Somebody";
	BOOST_CHECK(!(moneta::traits::extract_pk(dog) == moneta::traits::empty_pk<Dog>()));
}

BOOST_AUTO_TEST_CASE(has_empty_pk_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK(moneta::traits::has_empty_pk(person));

	person.ID = 555;
	BOOST_CHECK(!moneta::traits::has_empty_pk(person));


	Dog dog = moneta::make_entity<Dog>();
	BOOST_CHECK(moneta::traits::has_empty_pk(dog));

	dog.Owner = "Somebody";
	BOOST_CHECK(!moneta::traits::has_empty_pk(dog));
}
