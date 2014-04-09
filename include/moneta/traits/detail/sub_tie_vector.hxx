#pragma once
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Tuple, class Indices, const int N>
	struct tuple_element_reference : boost::add_reference<
		typename boost::mpl::at_c<
			Tuple,
			boost::mpl::at_c<Indices, N>::type::value
		>::type
	> {
		// Indices must be less then or equal to Tuple's size.
		BOOST_MPL_ASSERT_RELATION(boost::mpl::size<Indices>::value, <=, boost::mpl::size<Tuple>::value);
	};

	template <class Tuple, class Indices, const int IndicesSize = boost::mpl::size<Indices>::value>
	struct sub_tie_vector;
	
	template <class Tuple, class Indices>
	struct sub_tie_vector<Tuple, Indices, 1> {
		typedef typename boost::fusion::vector<
			typename tuple_element_reference<Tuple, Indices, 0>::type
		> type;

		type operator()(Tuple& tuple) const {
			return type(boost::fusion::at_c<0>(tuple));
		}
	};

	template <class Tuple, class Indices>
	struct sub_tie_vector<Tuple, Indices, 2> {
		typedef typename boost::fusion::vector<
			typename tuple_element_reference<Tuple, Indices, 0>::type,
			typename tuple_element_reference<Tuple, Indices, 1>::type
		> type;
	};

	template <class Tuple, class Indices>
	struct sub_tie_vector<Tuple, Indices, 3> {
		typedef typename boost::fusion::vector<
			typename tuple_element_reference<Tuple, Indices, 0>::type,
			typename tuple_element_reference<Tuple, Indices, 1>::type,
			typename tuple_element_reference<Tuple, Indices, 2>::type
		> type;
	};

	template <class Tuple, class Indices>
	struct sub_tie_vector<Tuple, Indices, 4> {
		typedef typename boost::fusion::vector<
			typename tuple_element_reference<Tuple, Indices, 0>::type,
			typename tuple_element_reference<Tuple, Indices, 1>::type,
			typename tuple_element_reference<Tuple, Indices, 2>::type,
			typename tuple_element_reference<Tuple, Indices, 3>::type
		> type;
	};
}}}
