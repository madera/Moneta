#include "stdafx.h"
#include "../model/Person.hxx"
#include <moneta/traits/member_ordinal.hxx>

#include <boost/mpl/find_if.hpp>
#include <boost/mpl/distance.hpp>

BOOST_AUTO_TEST_CASE(member_ordinal_test) {
	BOOST_CHECK_EQUAL((moneta::traits::member_ordinal<MONETA_MEMBER(Person, int,         ID     )>()), 0);
	BOOST_CHECK_EQUAL((moneta::traits::member_ordinal<MONETA_MEMBER(Person, std::string, Name   )>()), 1);
	BOOST_CHECK_EQUAL((moneta::traits::member_ordinal<MONETA_MEMBER(Person, double,      Height )>()), 2);
	BOOST_CHECK_EQUAL((moneta::traits::member_ordinal<MONETA_MEMBER(Person, int,         Fingers)>()), 3);
}
