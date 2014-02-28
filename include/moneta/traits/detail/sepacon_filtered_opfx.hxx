#pragma once
#include <boost/mpl/size.hpp>

namespace moneta { namespace traits { namespace detail {

	template <typename T>
	struct blank_filter {
		T operator()(T& x) {
			return x;
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter = blank_filter, int Size = boost::mpl::size<S>::value>
	struct sepacon_filtered_opfx;

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_filtered_opfx<S, K, P, Filter, 3> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p),
				boost::mpl::at_c<S, 1>::type()(p),

				Filter<typename boost::mpl::at_c<S, 2>::type::result_type>()
				(boost::mpl::at_c<S, 2>::type()(p))
			);
		}
	};

}}}
