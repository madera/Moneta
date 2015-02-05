#pragma once
#include <boost/mpl/clear.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>

namespace mplx {

	template<class Sequence> 
	struct flatten : boost::mpl::fold<
		Sequence,
		typename boost::mpl::clear<Sequence>::type,
		boost::mpl::if_<
			boost::mpl::is_sequence<boost::mpl::_2>,
			boost::mpl::copy<
				mplx::flatten<boost::mpl::_2>,
				boost::mpl::back_inserter<boost::mpl::_1>
			>,
			boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
		>
	> {};

}
