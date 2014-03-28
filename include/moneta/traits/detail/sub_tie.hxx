#pragma once
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/fusion/adapted.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Tuple, class Indices, const int N>
	struct tuple_element_reference : boost::add_reference<
		typename boost::tuples::element<
			boost::mpl::at_c<Indices, N>::type::value,
			Tuple
		>::type
	> {
		BOOST_MPL_ASSERT_RELATION(boost::mpl::size<Indices>::value, <=, boost::mpl::size<Tuple>::value);
	};

	template <class Tuple, class Indices, const int IndicesSize = boost::mpl::size<Indices>::value>
	struct sub_tie;
	
	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 1> : boost::tuple<
		typename tuple_element_reference<Tuple, Indices, 0>::type
	> {
		sub_tie(Tuple& x)
		 : tuple(
			boost::get<0>(x)
		) {}
	};

	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 2> : boost::tuple<
		typename tuple_element_reference<Tuple, Indices, 0>::type,
		typename tuple_element_reference<Tuple, Indices, 1>::type
	> {
		sub_tie(Tuple& x)
		 : tuple(
			boost::get<0>(x),
			boost::get<1>(x)
		) {}
	};

	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 3> : boost::tuple<
		typename tuple_element_reference<Tuple, Indices, 0>::type,
		typename tuple_element_reference<Tuple, Indices, 1>::type,
		typename tuple_element_reference<Tuple, Indices, 2>::type
	> {
		sub_tie(Tuple& x)
		 : tuple(
			boost::get<0>(x),
			boost::get<1>(x),
			boost::get<2>(x)
		) {}
	};

	template <class Tuple, class Indices>
	struct sub_tie<Tuple, Indices, 4> : boost::tuple<
		typename tuple_element_reference<Tuple, Indices, 0>::type,
		typename tuple_element_reference<Tuple, Indices, 1>::type,
		typename tuple_element_reference<Tuple, Indices, 3>::type,
		typename tuple_element_reference<Tuple, Indices, 2>::type
	> {
		sub_tie(Tuple& x)
		 : tuple(
			boost::get<0>(x),
			boost::get<1>(x),
			boost::get<2>(x),
			boost::get<3>(x)
		) {}
	};

}}}
