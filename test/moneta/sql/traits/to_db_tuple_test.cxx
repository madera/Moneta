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

	moneta::sql::traits::db_tuple<Cat>::type db_tuple =
		moneta::sql::traits::to_db_tuple<Cat>(cat);

	BOOST_CHECK_EQUAL(boost::fusion::at_c<0>(db_tuple), 555);
	BOOST_CHECK_EQUAL(boost::fusion::at_c<1>(db_tuple), "Garfield");
	BOOST_CHECK_EQUAL(boost::fusion::at_c<2>(db_tuple), 255);
}
