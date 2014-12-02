#pragma once
#include <boost/mpl/clear.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/vector.hpp>

namespace mplx {
	namespace detail {
		struct joint_view {
			template <class S0, class S1>
			struct apply : boost::mpl::joint_view<S0, S1> {};
		};
	}

	template<class Sequence> 
	struct flatten : boost::mpl::fold<
		Sequence,
		typename boost::mpl::clear<Sequence>::type,
		detail::joint_view
	> {};
}
