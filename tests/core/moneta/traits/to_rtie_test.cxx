#include "stdafx.h"
#include <moneta/traits/to_rtie.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Composite.hxx"
#include "../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(to_rtie_test) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Street = 1;
	cat.Address.Street = "Infinite Loop";

	moneta::traits::rtie<Cat>::type rtie =
		moneta::traits::to_rtie<Cat>(cat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtie), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtie), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(rtie), 255);
}

BOOST_AUTO_TEST_CASE(to_rtie_2_test) {
	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Ratings = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";

	moneta::traits::rtie<Composite>::type rtie =
		moneta::traits::to_rtie<Composite>(composite);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtie), 2600);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtie), 123);

	const moneta::traits::pk<Dog>::type dog_pk = boost::fusion::at_c<2>(rtie);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(dog_pk), "Someowner");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(dog_pk), 555);
}

BOOST_AUTO_TEST_CASE(const_to_rtie_test) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Street = 1;
	cat.Address.Street = "Infinite Loop";

	moneta::traits::rtie<Cat>::type rtie =
		moneta::traits::to_rtie<Cat>(cat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtie), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtie), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(rtie), 255);
}
