#include "stdafx.h"
#include <moneta/sql/traits/db_tie.hxx>
#include <boost/mpl/equal.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/Cat.hxx"

static void static_test() {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Person>::type,
			boost::mpl::vector<int&, std::string&, double&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<const Person>::type,
			boost::mpl::vector<const int&, const std::string&, const double&, const int&>
		>
	));

	// Cat
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Cat>::type,
			boost::mpl::vector<int&, std::string&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<const Cat>::type,
			boost::mpl::vector<const int&, const std::string&, const int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Dog>::type,
			boost::mpl::vector3<std::string&, int&, std::string&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<const Dog>::type,
			boost::mpl::vector3<const std::string&, const int&, const std::string&>
		>
	));

	// Composite
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Composite>::type,
			boost::fusion::vector3<int&, int&, boost::fusion::vector2<std::string&, int&> >
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<const Composite>::type,
			boost::fusion::vector3<const int&, const int&,
				boost::fusion::vector2<const std::string&, const int&> >
		>
	));
}

BOOST_AUTO_TEST_CASE(db_tie_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Person>::type,
			boost::mpl::vector4<int&, std::string&, double&, int&>
		>
	));
}
