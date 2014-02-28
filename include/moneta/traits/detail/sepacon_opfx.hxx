#pragma once
#include <boost/mpl/size.hpp>

namespace moneta { namespace traits { namespace detail {

	template <typename T>
	struct sepacon_opfx_blank_filter {
		T& operator()(T& x) {
			return x;
		}

		const T& operator()(const T& x) {
			return x;
		}
	};

	template <
		class S,
		class K,
		typename P,
		template <typename T> class Filter = sepacon_opfx_blank_filter,
		int Size = boost::mpl::size<S>::value
	>
	struct sepacon_opfx;

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 0> {
		K operator()(P p) const {
			return K();
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 1> {
		K operator()(P p = nullptr) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(at_c<S, 0>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 2> {
		K operator()(P p = nullptr) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(at_c<S, 1>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 3> {
		K operator()(P p = nullptr) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(at_c<S, 2>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 4> {
		K operator()(P p = nullptr) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(at_c<S, 3>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 5> {
		K operator()(P p = nullptr) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(at_c<S, 4>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 6> {
		K operator()(P p = nullptr) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(at_c<S, 5>::type()(p))
			);
		}
	};

}}}