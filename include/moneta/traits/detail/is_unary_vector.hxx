#pragma once
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal_to.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Sequence>
	struct is_unary_vector : boost::mpl::equal_to<
		typename boost::mpl::size<Sequence>::type,
		boost::mpl::int_<1>
	> {};

}}}
