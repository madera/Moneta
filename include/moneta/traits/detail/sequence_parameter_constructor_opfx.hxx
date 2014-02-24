#pragma once
#include <boost/mpl/size.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class S, class K, typename P = void*, int Size = boost::mpl::size<S>::value>
	struct sequence_parameter_constructor_opfx;

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 0> {
		K operator()(P p) const {
			return K();
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 1> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p)
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 2> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p),
				boost::mpl::at_c<S, 1>::type()(p)
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 3> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p),
				boost::mpl::at_c<S, 1>::type()(p),
				boost::mpl::at_c<S, 2>::type()(p)
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 4> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p),
				boost::mpl::at_c<S, 1>::type()(p),
				boost::mpl::at_c<S, 2>::type()(p),
				boost::mpl::at_c<S, 3>::type()(p)
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 5> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p),
				boost::mpl::at_c<S, 1>::type()(p),
				boost::mpl::at_c<S, 2>::type()(p),
				boost::mpl::at_c<S, 3>::type()(p),
				boost::mpl::at_c<S, 4>::type()(p)
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_opfx<S, K, P, 6> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type()(p),
				boost::mpl::at_c<S, 1>::type()(p),
				boost::mpl::at_c<S, 2>::type()(p),
				boost::mpl::at_c<S, 3>::type()(p),
				boost::mpl::at_c<S, 4>::type()(p),
				boost::mpl::at_c<S, 5>::type()(p)
			);
		}
	};

}}}
