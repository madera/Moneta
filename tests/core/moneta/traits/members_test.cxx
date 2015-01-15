#include "stdafx.h"
#include "../model/Person.hxx"
#include "../model/tree/A.hxx"
#include <boost/mpl/at.hpp>

#include <boost/mpl/print.hpp>

inline void static_test() {

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::traits::members<Person>::type, 0>::type,
			MONETA_MEMBER(Person, int, ID)
		>
	));

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::traits::members<Person>::type, 1>::type,
			MONETA_MEMBER(Person, std::string, Name)
		>
	));

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::traits::members<Person>::type, 2>::type,
			MONETA_MEMBER(Person, double, Height)
		>
	));

	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::mpl::at_c<moneta::traits::members<Person>::type, 3>::type,
			MONETA_MEMBER(Person, int, Fingers)
		>
	));

	//
	// member_inequality_test
	//

	BOOST_MPL_ASSERT_NOT((
		boost::is_same<
			MONETA_MEMBER(Person, int, ID),
			MONETA_MEMBER(Person, int, Fingers)
		>
	));

	BOOST_MPL_ASSERT((moneta::traits::is_first_member<MONETA_MEMBER(Person, int, ID)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_member<MONETA_MEMBER(Person, int, Fingers)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_member<MONETA_MEMBER(Person, int, ID)>));
	BOOST_MPL_ASSERT((moneta::traits::is_last_member<MONETA_MEMBER(Person, int, Fingers)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_member_entity<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_member_entity<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_member_entity<MONETA_MEMBER(A, B  , b)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_member_entity<MONETA_MEMBER(A, int, h)>));

	BOOST_MPL_ASSERT    ((moneta::traits::is_first_non_entity_member<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_non_entity_member<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_non_entity_member<MONETA_MEMBER(A, int, h)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_non_entity_member<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_non_entity_member<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_last_non_entity_member<MONETA_MEMBER(A, int, h)>));

	BOOST_MPL_ASSERT((moneta::traits::is_first_entity_member<MONETA_MEMBER(A, B, b)>));
	BOOST_MPL_ASSERT((moneta::traits:: is_last_entity_member<MONETA_MEMBER(A, B, b)>));
	
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_entity_member<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_entity_member<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_entity_member<MONETA_MEMBER(A, int, h)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits:: is_last_entity_member<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits:: is_last_entity_member<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits:: is_last_entity_member<MONETA_MEMBER(A, int, h)>));
}