#pragma once
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

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
		K operator()(P) const {
			return K();
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 1> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 2> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 3> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 4> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 5> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 6> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 7> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 8> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 9> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 10> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 11> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 10>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 12> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 10>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 11>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 13> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 10>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 11>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 12>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 14> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 10>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 11>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 12>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 13>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 15> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 10>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 11>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 12>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 13>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 14>::type()(p))
			);
		}
	};

	template <class S, class K, typename P, template <typename T> class Filter>
	struct sepacon_opfx<S, K, P, Filter, 16> {
		K operator()(P p = 0) const {
			using namespace boost::mpl;
			return K(
				Filter<typename at_c<S, 0>::type::result_type>()(typename at_c<S, 0>::type()(p)),
				Filter<typename at_c<S, 1>::type::result_type>()(typename at_c<S, 1>::type()(p)),
				Filter<typename at_c<S, 2>::type::result_type>()(typename at_c<S, 2>::type()(p)),
				Filter<typename at_c<S, 3>::type::result_type>()(typename at_c<S, 3>::type()(p)),
				Filter<typename at_c<S, 4>::type::result_type>()(typename at_c<S, 4>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 5>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 6>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 7>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 8>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 9>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 10>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 11>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 12>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 13>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 14>::type()(p)),
				Filter<typename at_c<S, 5>::type::result_type>()(typename at_c<S, 15>::type()(p))
			);
		}
	};

}}}
