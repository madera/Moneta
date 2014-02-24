#include "stdafx.h"
#include <moneta/meta/tuple.hxx>
#include <boost/fusion/include/at_c.hpp>
#include <boost/mpl/at.hpp>
#include "../model/Person.hxx"

#include <boost/mpl/print.hpp>

BOOST_AUTO_TEST_CASE(tuple_types_test) {
	using boost::is_same;
	using boost::mpl::at_c;

	typedef moneta::meta::detail::mpl::vector<Person>::type mpl_vector_type;
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 0>::type, int        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 1>::type, std::string>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 2>::type, double     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 3>::type, int        >)); // Fingers

	typedef moneta::meta::tuple<Person>::type tuple_type;
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 0>::type, int        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 1>::type, std::string>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 2>::type, double     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 3>::type, int        >)); // Fingers

	typedef moneta::meta::tie<Person>::type tie_type;
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 0>::type, int&        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 1>::type, std::string&>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 2>::type, double&     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 3>::type, int&        >)); // Fingers

	typedef moneta::meta::const_tie<Person>::type const_tie_type;
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 0>::type, const int&        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 1>::type, const std::string&>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 2>::type, const double&     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 3>::type, const int&        >)); // Fingers
}

BOOST_AUTO_TEST_CASE(tuple_test) {
	moneta::meta::tuple<Person>::type tuple;

}
