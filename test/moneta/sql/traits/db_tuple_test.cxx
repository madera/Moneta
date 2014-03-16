#include "stdafx.h"
#include <moneta/sql/traits/db_tuple.hxx>
#include <boost/mpl/equal.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/Cat.hxx"

static void static_test() {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<const Person>::type,
			boost::mpl::vector<const int, const std::string, const double, const int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<Cat>::type,
			boost::mpl::vector<int, std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<const Cat>::type,
			boost::mpl::vector<const int, const std::string, const int>
		>
	));
}

BOOST_AUTO_TEST_CASE(db_tuple_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<Dog>::type,
			boost::mpl::vector<std::string, int, std::string>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<Composite>::type,
			boost::fusion::vector3<int, int, boost::fusion::vector2<std::string, int> >
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tuple<Cat>::type,
			boost::fusion::vector3<int, std::string, int>
		>
	));
}
