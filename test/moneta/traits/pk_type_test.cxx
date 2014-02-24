#include "stdafx.h"
#include <moneta/traits/pk_type.hxx>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(pk_type_test) {
	BOOST_MPL_ASSERT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, int, ID)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, std::string, Name   )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, double,      Height )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, int,         Fingers)>));
}
