#include "stdafx.h"
#include <moneta/sql/traits/rtuple.hxx>
#include <boost/mpl/equal.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/Cat.hxx"

static void static_test() {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<const Person>::type,
			boost::mpl::vector<const int, const std::string, const double, const int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<Cat>::type,
			boost::mpl::vector<int, std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<const Cat>::type,
			boost::mpl::vector<const int, const std::string, const int>
		>
	));
}

BOOST_AUTO_TEST_CASE(rtuple_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<Dog>::type,
			boost::mpl::vector<std::string, int, std::string>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<Composite>::type,
			boost::fusion::vector3<int, int, boost::fusion::vector2<std::string, int> >
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::rtuple<Cat>::type,
			boost::fusion::vector3<int, std::string, int>
		>
	));
}
