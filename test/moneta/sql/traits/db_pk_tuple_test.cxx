#include "stdafx.h"
#include <moneta/sql/traits/db_pk_tuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../../model/Cat.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Lizard.hxx"

static void static_test() {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_pk_tuple<Cat>::type,
			int
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_pk_tuple<Dog>::type,
			boost::fusion::vector<std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::db_pk_tuple<Lizard>::type,
			boost::fusion::vector<int, int>
		>
	));
}
