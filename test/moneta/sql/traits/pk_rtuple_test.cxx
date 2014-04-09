#include "stdafx.h"
#include <moneta/sql/traits/pk_rtuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../../model/Cat.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Lizard.hxx"

static void static_test() {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtuple<Cat>::type,
			int
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtuple<Dog>::type,
			boost::fusion::vector<std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtuple<Lizard>::type,
			boost::fusion::vector<int, int>
		>
	));
}

BOOST_AUTO_TEST_CASE(pk_rtuple_extraction_test) {
	int x = 0;
}
