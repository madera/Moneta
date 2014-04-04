#include "stdafx.h"
#include <moneta/sql/traits/pk_rtie.hxx>
#include <boost/mpl/equal.hpp>
#include "../../model/Cat.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Lizard.hxx"

static void static_test() {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtie<Cat>::type,
			int&
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtie<Dog>::type,
			boost::fusion::vector<std::string&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtie<Lizard>::type,
			boost::fusion::vector<int&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::sql::traits::pk_rtie<const Cat>::type,
			const int&
		>
	));
}
