#include "stdafx.h"
#include "../model/Person.hxx"
#include <boost/mpl/at.hpp>

BOOST_AUTO_TEST_CASE(members_of_test) {
	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::meta::members_of<Person>::type, 0>::type,
			MONETA_MEMBER(Person, int, ID)
		>
	));

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::meta::members_of<Person>::type, 1>::type,
			MONETA_MEMBER(Person, std::string, Name)
		>
	));

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::meta::members_of<Person>::type, 2>::type,
			MONETA_MEMBER(Person, double, Height)
		>
	));

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::meta::members_of<Person>::type, 3>::type,
			MONETA_MEMBER(Person, int, Fingers)
		>
	));
}

BOOST_AUTO_TEST_CASE(members_of_inequality_test) {
	BOOST_MPL_ASSERT_NOT((
		boost::is_same<
			MONETA_MEMBER(Person, int, ID),
			MONETA_MEMBER(Person, int, Fingers)
		>
	));
}
