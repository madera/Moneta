// TODO: Boost.PPize this thing. Must support C++03.
#pragma once
#include <boost/mpl/size.hpp>

namespace moneta { namespace meta { namespace detail {

	template <class S, class K, typename P = void*, int Size = boost::mpl::size<S>::value>
	struct sequence_parameter_constructor_sg;

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 0> {
		K operator()(P p) const {
			return K();
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 1> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type::get()
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 2> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type::get(),
				boost::mpl::at_c<S, 1>::type::get()
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 3> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type::get(),
				boost::mpl::at_c<S, 1>::type::get(),
				boost::mpl::at_c<S, 2>::type::get()
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 4> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type::get(),
				boost::mpl::at_c<S, 1>::type::get(),
				boost::mpl::at_c<S, 2>::type::get(),
				boost::mpl::at_c<S, 3>::type::get()
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 5> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type::get(),
				boost::mpl::at_c<S, 1>::type::get(),
				boost::mpl::at_c<S, 2>::type::get(),
				boost::mpl::at_c<S, 3>::type::get(),
				boost::mpl::at_c<S, 4>::type::get()
			);
		}
	};

	template <class S, class K, typename P>
	struct sequence_parameter_constructor_sg<S, K, P, 6> {
		K operator()(P p = nullptr) const {
			return K(
				boost::mpl::at_c<S, 0>::type::get(),
				boost::mpl::at_c<S, 1>::type::get(),
				boost::mpl::at_c<S, 2>::type::get(),
				boost::mpl::at_c<S, 3>::type::get(),
				boost::mpl::at_c<S, 4>::type::get(),
				boost::mpl::at_c<S, 5>::type::get()
			);
		}
	};

}}}
