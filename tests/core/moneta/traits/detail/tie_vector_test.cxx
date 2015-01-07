#include "stdafx.h"
#include <moneta/traits/detail/tie_vector.hxx>
#include <boost/tuple/tuple.hpp>

BOOST_AUTO_TEST_CASE(tie_vector_with_boost_tuple_test) {

	typedef boost::tuple<char , short , int > tuple_t;
		
	tuple_t tuple(rand() % 128, rand() % 32768, rand() % 100000);

	boost::fusion::vector<char&, short&, int&> tie = moneta::traits::detail::tie_vector(tuple);
	
	using boost::fusion::at_c;
	BOOST_CHECK_EQUAL(at_c<0>(tie), at_c<0>(tuple));
	BOOST_CHECK_EQUAL(at_c<1>(tie), at_c<1>(tuple));
	BOOST_CHECK_EQUAL(at_c<2>(tie), at_c<2>(tuple));
}

BOOST_AUTO_TEST_CASE(tie_vector_with_fusion_vector_test) {

	typedef boost::fusion::vector<char , short , int > tuple_t;
	typedef boost::fusion::vector<char&, short&, int&>   tie_t;
		
	tuple_t tuple(rand() % 128, rand() % 32768, rand() % 100000);

	tie_t tie = moneta::traits::detail::tie_vector(tuple);
	
	using boost::fusion::at_c;
	BOOST_CHECK_EQUAL(at_c<0>(tie), at_c<0>(tuple));
	BOOST_CHECK_EQUAL(at_c<1>(tie), at_c<1>(tuple));
	BOOST_CHECK_EQUAL(at_c<2>(tie), at_c<2>(tuple));
}
