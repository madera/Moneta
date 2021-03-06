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
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include <moneta/traits/extract_pk.hxx>
#include <moneta/traits/same_pk.hxx>

BOOST_AUTO_TEST_CASE(extract_pk_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Ratings = 10;

	moneta::traits::extract_pk(person) = 555;
	BOOST_CHECK_EQUAL(person.ID, 555);

	Dog dog;
	dog.Owner = "Charlie";
	dog.ID = 1;
	dog.Name = "Snoopy";

	boost::fusion::at_c<1>(moneta::traits::extract_pk(dog)) = 5;
	BOOST_CHECK_EQUAL(dog.ID, 5);
}

BOOST_AUTO_TEST_CASE(const_extract_pk_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Ratings = 10;

	const Person& const_person_a = person;
	BOOST_CHECK_EQUAL(moneta::traits::extract_pk(const_person_a), 1);

	person.ID = 123;
	BOOST_CHECK_EQUAL(moneta::traits::extract_pk(const_person_a), 123);

	const Person& const_person_b = person;
	BOOST_CHECK_EQUAL(moneta::traits::extract_pk(const_person_b), 123);

	Dog dog;
	dog.Owner = "Charlie";
	dog.ID = 1;
	dog.Name = "Snoopy";
	
	const Dog& const_dog = dog;
	moneta::traits::pk_tie<const Dog>::type cpk_tie = moneta::traits::extract_pk(const_dog);
	
	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(cpk_tie), "Charlie");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(cpk_tie), 1);

	// Alternate syntax... TODO: Just keep one of the two: using .mN or at_c<N>().
	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(moneta::traits::extract_pk(const_dog)), "Charlie");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(moneta::traits::extract_pk(const_dog)), 1);
}

BOOST_AUTO_TEST_CASE(same_pk_test) {

	//
	// Person
	//

	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Ratings = 10;
	BOOST_CHECK(moneta::traits::same_pk(person, person));

	Person person2;
	person2.ID = 1;
	BOOST_CHECK(moneta::traits::same_pk(person, person2));

	person2.ID = 5;
	BOOST_CHECK(!moneta::traits::same_pk(person, person2));
	
	//
	// Dog
	//

	Dog dog;
	dog.Owner = "Charlie";
	dog.ID = 1;
	dog.Name = "Snoopy";	
	BOOST_CHECK(moneta::traits::same_pk(dog, dog));

	Dog dog2;
	dog2.Owner = "Charlie";
	dog2.ID = 1;
	dog.Name = "Unknown";
	BOOST_CHECK(moneta::traits::same_pk(dog, dog2));

	dog2.ID = 5;
	BOOST_CHECK(!moneta::traits::same_pk(dog, dog2));
}
