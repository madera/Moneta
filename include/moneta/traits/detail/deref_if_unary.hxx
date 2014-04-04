#pragma once
#include <boost/fusion/view/transform_view.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal_to.hpp>

namespace moneta { namespace traits { namespace detail {

	//template <class Sequence>
	//struct deref_if_unary : boost::mpl::if_<
	//	boost::mpl::equal_to<
	//		typename boost::mpl::size<Sequence>::type,
	//		boost::mpl::int_<1>
	//	>,
	//	typename boost::mpl::at_c<Sequence, 0>::type,
	//	Sequence
	//> {};

	struct element_referencer_transformer {
		template<typename Sig>
		struct result;

		template<typename T>
		struct result<element_referencer_transformer(T&)>
		 : boost::add_reference<T> {};

		template<typename T>
		typename boost::add_reference<T>::type
		operator()(T& x) const {
			return x;
		}
	};

	template <class Sequence>
	boost::fusion::transform_view<Sequence, element_referencer_transformer>
	inline make_reference_transformer(Sequence& sequence) {
		return boost::fusion::transform_view<Sequence, element_referencer_transformer>(
			sequence,
			element_referencer_transformer()
		);
	}

	// Type: vector1<T> --> T, else: Sequence.
	//
	template <
		class Sequence,
		const bool IsSequence = boost::mpl::equal_to<
			typename boost::mpl::size<Sequence>::type,
			boost::mpl::int_<1>
		>::type::value
	>
	struct deref_if_unary;

	template <class Sequence>
	struct deref_if_unary<Sequence, true> {
		typedef typename boost::mpl::at_c<Sequence, 0>::type type;

		type operator()(Sequence& s) const {
			return boost::fusion::at_c<0>(s);
		}

		type operator()(const Sequence& s) const {
			return boost::fusion::at_c<0>(s);
		}
	};

	template <class Sequence>
	struct deref_if_unary<Sequence, false> {
		typedef Sequence type;

		type& operator()(Sequence& s) const {
			return s;
		}

		//const type& operator()(const Sequence& s) const {
		//	return s;
		//}
	};

}}}
