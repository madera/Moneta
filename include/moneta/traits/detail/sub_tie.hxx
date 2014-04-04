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
		BOOST_MPL_ASSERT_RELATION(boost::mpl::size<Indices>::value, <=, boost::mpl::size<Tuple>::value);
	};

	template <class Tuple, class Indices, const int IndicesSize = boost::mpl::size<Indices>::value>
	struct sub_tie;
	
	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 1> : boost::fusion::vector<
		typename tuple_element_reference<Tuple, Indices, 0>::type
	> {
		sub_tie(Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x)
		) {}

		sub_tie(const Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x)
		) {}
	};

	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 2> : boost::fusion::vector<
		typename tuple_element_reference<Tuple, Indices, 0>::type,
		typename tuple_element_reference<Tuple, Indices, 1>::type
	> {
		sub_tie(Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x),
			boost::fusion::at_c<1>(x)
		) {}

		sub_tie(const Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x),
			boost::fusion::at_c<1>(x)
		) {}
	};

	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 3> : boost::fusion::vector<
		typename tuple_element_reference<Tuple, Indices, 0>::type,
		typename tuple_element_reference<Tuple, Indices, 1>::type,
		typename tuple_element_reference<Tuple, Indices, 2>::type
	> {
		sub_tie(Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x),
			boost::fusion::at_c<1>(x),
			boost::fusion::at_c<2>(x)
		) {}

		sub_tie(const Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x),
			boost::fusion::at_c<1>(x),
			boost::fusion::at_c<2>(x)
		) {}
	};

	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 4> : boost::fusion::vector<
		typename tuple_element_reference<Tuple, Indices, 0>::type,
		typename tuple_element_reference<Tuple, Indices, 1>::type,
		typename tuple_element_reference<Tuple, Indices, 3>::type,
		typename tuple_element_reference<Tuple, Indices, 2>::type
	> {
		sub_tie(Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x),
			boost::fusion::at_c<1>(x),
			boost::fusion::at_c<2>(x),
			boost::fusion::at_c<3>(x)
		) {}

		sub_tie(const Tuple& x)
		 : vector(
			boost::fusion::at_c<0>(x),
			boost::fusion::at_c<1>(x),
			boost::fusion::at_c<2>(x),
			boost::fusion::at_c<3>(x)
		) {}
	};

}}}

// TODO: Add more variations.
