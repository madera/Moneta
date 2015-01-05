#include "stdafx.h"
#include <moneta/_aux/mplx_flatten.hxx>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/print.hpp>

inline void static_test() {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			mplx::flatten<
				boost::mpl::vector<
					boost::mpl::vector<char, short, double>,
					boost::mpl::vector<float, bool>
				>
			>::type,
			boost::mpl::vector<char, short, double, float, bool>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			mplx::flatten<
				boost::mpl::vector<
					boost::mpl::vector<char>,
					boost::mpl::vector<>
				>
			>::type,
			boost::mpl::vector<char>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			mplx::flatten<
				boost::mpl::vector<
					boost::mpl::vector<int*, char*>,
					boost::mpl::vector<int, char>
				>
			>::type,
			boost::mpl::vector<int*, char*, int, char>
		>
	));
}
