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
#include <moneta/traits/pk_rtuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"
#include "../model/Lizard.hxx"

inline void static_test() {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_rtuple<Cat>::type,
			int
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_rtuple<Dog>::type,
			boost::fusion::vector<std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_rtuple<Lizard>::type,
			boost::fusion::vector<int, int>
		>
	));
}
