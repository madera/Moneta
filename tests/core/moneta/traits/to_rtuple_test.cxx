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
#include <moneta/traits/to_rtuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Composite.hxx"
#include "../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(to_rtuple_test) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Street = 1;
	cat.Address.Street = "Infinite Loop";

	moneta::traits::rtuple<Cat>::type rtuple =
		moneta::traits::to_rtuple<Cat>(cat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtuple), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtuple), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(rtuple), 255);
}

BOOST_AUTO_TEST_CASE(to_rtuple_2_test) {
	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Ratings = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";

	moneta::traits::rtuple<Composite>::type rtuple =
		moneta::traits::to_rtuple<Composite>(composite);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtuple), 2600);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtuple), 123);

	const moneta::traits::pk<Dog>::type dog_pk = boost::fusion::at_c<2>(rtuple);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(dog_pk), "Someowner");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(dog_pk), 555);
}

BOOST_AUTO_TEST_CASE(const_to_rtuple_test) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Number = 1;
	cat.Address.Street = "Infinite Loop";

	const Cat& ccat = cat;

	moneta::traits::rtuple<const Cat>::type rtuple =
		moneta::traits::to_rtuple<const Cat>(ccat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtuple), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtuple), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(rtuple), 255);

	moneta::traits::rtuple<Cat>::type non_const_tuple = 
		moneta::traits::to_rtuple<const Cat>(ccat);
}
