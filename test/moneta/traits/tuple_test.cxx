#include "stdafx.h"
#include <moneta/traits/tuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"

static void static_test() {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::const_tuple<Person>::type,
			boost::mpl::vector<const int, const std::string, const double, const int>
		>
	));
}

BOOST_AUTO_TEST_CASE(tuple_types_test) {
	using boost::is_same;
	using boost::mpl::at_c;

	typedef moneta::traits::detail::mpl::vector<Person>::type mpl_vector_type;
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 0>::type, int        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 1>::type, std::string>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 2>::type, double     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 3>::type, int        >)); // Fingers

	typedef moneta::traits::tuple<Person>::type tuple_type;
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 0>::type, int        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 1>::type, std::string>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 2>::type, double     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 3>::type, int        >)); // Fingers
}
