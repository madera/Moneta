#include "stdafx.h"
#include <moneta/sql/traits/to_db_tuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(to_db_tuple_test) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Street = 1;
	cat.Address.Street = "Infinite Loop";

	moneta::sql::traits::rtuple<Cat>::type rtuple =
		moneta::sql::traits::to_db_tuple<Cat>(cat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtuple), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtuple), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(rtuple), 255);
}

BOOST_AUTO_TEST_CASE(to_db_tuple_2_test) {
	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Fingers = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";

	moneta::sql::traits::rtuple<Composite>::type rtuple =
		moneta::sql::traits::to_db_tuple<Composite>(composite);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtuple), 2600);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtuple), 123);

	const moneta::traits::pk<Dog>::type dog_pk = boost::fusion::at_c<2>(rtuple);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(dog_pk), "Someowner");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(dog_pk), 555);
}

BOOST_AUTO_TEST_CASE(const_to_db_tuple_test) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Number = 1;
	cat.Address.Street = "Infinite Loop";

	const Cat& ccat = cat;

	moneta::sql::traits::rtuple<const Cat>::type rtuple =
		moneta::sql::traits::to_db_tuple<const Cat>(ccat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(rtuple), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(rtuple), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(rtuple), 255);

	moneta::sql::traits::rtuple<Cat>::type non_const_tuple = 
		moneta::sql::traits::to_db_tuple<const Cat>(ccat);
}
