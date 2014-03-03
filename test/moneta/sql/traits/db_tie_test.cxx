#include "stdafx.h"
#include <moneta/sql/traits/db_tie.hxx>
#include <boost/mpl/equal.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(db_tie_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Person>::type,
			boost::mpl::vector4<int&, std::string&, double&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Dog>::type,
			boost::mpl::vector3<std::string&, int&, std::string&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_tie<Composite>::type,
			boost::fusion::vector3<int&, int&, boost::fusion::vector2<std::string&, int&> >
		>
	));
}
