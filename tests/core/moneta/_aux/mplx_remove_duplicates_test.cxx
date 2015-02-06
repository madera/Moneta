#include "stdafx.h"
#include <moneta/_aux/mplx_remove_duplicates.hxx>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>

inline void static_test() {

	BOOST_MPL_ASSERT((boost::mpl::equal<
		typename mplx::remove_duplicates<
			boost::mpl::vector0<>
		>::type,
		boost::mpl::vector0<>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		typename mplx::remove_duplicates<
			boost::mpl::vector1<char>
		>::type,
		boost::mpl::vector1<char>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		typename mplx::remove_duplicates<
			boost::mpl::vector2<char, char>
		>::type,
		boost::mpl::vector1<char>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		typename mplx::remove_duplicates<
			boost::mpl::vector3<char, char, char>
		>::type,
		boost::mpl::vector1<char>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		typename mplx::remove_duplicates<
			boost::mpl::vector<char, short, int, char, short, int>
		>::type,
		boost::mpl::vector<char, short, int>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		typename mplx::remove_duplicates<
			boost::mpl::vector<char, short, char, int, char, short, short, int, double, float, double>
		>::type,
		boost::mpl::vector<char, short, int, double, float>
	>));

}
