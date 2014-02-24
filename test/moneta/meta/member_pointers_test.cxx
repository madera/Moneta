#include "stdafx.h"
#include <moneta/meta/member_pointers.hxx>
#include <boost/fusion/include/at_c.hpp>
#include <boost/mpl/at.hpp>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(member_pointers_type_test) {
	using boost::is_same;
	using boost::mpl::at_c;

	typedef moneta::meta::member_pointers<Person>::type type;

	BOOST_MPL_ASSERT((is_same<at_c<type, 0>::type, int         Person::*>)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<type, 1>::type, std::string Person::*>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<type, 2>::type, double      Person::*>)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<type, 3>::type, int         Person::*>)); // Fingers
}

BOOST_AUTO_TEST_CASE(member_pointers_test) {
	using moneta::meta::member_pointers;
	member_pointers<Person>::type memptrs = member_pointers<Person>::get();

	using boost::fusion::at_c;
	BOOST_CHECK_EQUAL(at_c<0>(memptrs), &Person::ID);
	BOOST_CHECK_EQUAL(at_c<1>(memptrs), &Person::Name);
	BOOST_CHECK_EQUAL(at_c<2>(memptrs), &Person::Height);
	BOOST_CHECK_EQUAL(at_c<3>(memptrs), &Person::Fingers);
}
