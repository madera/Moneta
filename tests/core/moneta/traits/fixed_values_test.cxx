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
#include <moneta/traits/fixed_values.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"

inline void static_test() {
	BOOST_MPL_ASSERT((moneta::traits::fixed_value<MONETA_MEMBER(ThreeInts, int, One)>));
	BOOST_MPL_ASSERT((moneta::traits::fixed_value<MONETA_MEMBER(ThreeInts, int, Two)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::fixed_value<MONETA_MEMBER(ThreeInts, int, Three)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::fixed_value<MONETA_MEMBER(FourInts, int, One  )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::fixed_value<MONETA_MEMBER(FourInts, int, Two  )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::fixed_value<MONETA_MEMBER(FourInts, int, Three)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::fixed_value<MONETA_MEMBER(FourInts, int, Four )>));
}
