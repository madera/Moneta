// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include <moneta/_aux/mplx_flatten.hxx>
#include <boost/mpl/equal.hpp>

inline void static_test() {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			mplx::flatten<
				boost::mpl::vector<
					boost::mpl::vector<char, short, double, float>,
					boost::mpl::vector<float, bool>
				>
			>::type,
			boost::mpl::vector<char, short, double, float, float, bool>
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
