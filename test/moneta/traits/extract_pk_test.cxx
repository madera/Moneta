#include "stdafx.h"
#include <moneta/traits/extract_pk.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

#include <boost/utility/result_of.hpp>
#include <boost/mpl/print.hpp>

BOOST_AUTO_TEST_CASE(extract_pk_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Fingers = 10;

	moneta::traits::extract_pk(person) = 555;
	BOOST_CHECK_EQUAL(person.ID, 555);

	Dog dog;
	dog.Owner = "Charlie";
	dog.ID = 1;
	dog.Name = "Snoopy";

	moneta::traits::extract_pk(dog).m0 = 5;
	//BOOST_CHECK_EQUAL(dog.ID, 5);
}

BOOST_AUTO_TEST_CASE(const_extract_pk_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Fingers = 10;

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
	moneta::traits::const_pk_tie<Dog>::type const_pk_tie = moneta::traits::extract_pk(const_dog);
	
	BOOST_CHECK_EQUAL(const_pk_tie.m0, "Charlie");
	BOOST_CHECK_EQUAL(const_pk_tie.m1, 1);

	// Alternate syntax... TODO: Just keep one of the two: using .mN or at_c<N>().
	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(moneta::traits::extract_pk(const_dog)), "Charlie");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(moneta::traits::extract_pk(const_dog)), 1);
}
