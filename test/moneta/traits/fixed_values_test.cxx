#include "stdafx.h"
#include <moneta/traits/fixed_values.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"

static void static_test() {
	BOOST_MPL_ASSERT((moneta::traits::detail::fixed_value<MONETA_MEMBER(ThreeInts, int, One)>));
	BOOST_MPL_ASSERT((moneta::traits::detail::fixed_value<MONETA_MEMBER(ThreeInts, int, Two)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::fixed_value<MONETA_MEMBER(ThreeInts, int, Three)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::fixed_value<MONETA_MEMBER(FourInts, int, One  )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::fixed_value<MONETA_MEMBER(FourInts, int, Two  )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::fixed_value<MONETA_MEMBER(FourInts, int, Three)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::fixed_value<MONETA_MEMBER(FourInts, int, Four )>));
}
