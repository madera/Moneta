#include "stdafx.h"
#include <moneta/traits/pk.hxx>
#include "../model/Person.hxx"
#include <boost/mpl/equal.hpp>

BOOST_AUTO_TEST_CASE(pk_type_test) {
	BOOST_MPL_ASSERT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, int, ID)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, std::string, Name   )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, double,      Height )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, int,         Fingers)>));
}

struct Dummy {
};

BOOST_AUTO_TEST_CASE(has_pk_test) {
	BOOST_MPL_ASSERT((moneta::traits::has_pk<Person>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::has_pk<Dummy>));
}

BOOST_AUTO_TEST_CASE(detail_mpl_pk_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::pk<Person>::type,
			boost::mpl::vector<int>
		>
	));
}

BOOST_AUTO_TEST_CASE(detail_fusion_pk_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk<Person>::type,
			boost::fusion::vector<int>
		>
	));
}


BOOST_AUTO_TEST_CASE(pk_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk<Person>::type,
			int
		>
	));
}
