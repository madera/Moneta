#pragma once
#include <boost/fusion/view/transform_view.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/mpl/print.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Sequence>
	struct is_unary_vector : boost::mpl::equal_to<
		typename boost::mpl::size<Sequence>::type,
		boost::mpl::int_<1>
	> {};

	template <class Sequence, bool Unary = is_unary_vector<Sequence>::value>
	struct deref_if_unary_result;

	template <class Sequence>
	struct deref_if_unary_result<Sequence, true> {
		typedef typename boost::mpl::at_c<Sequence, 0>::type type;
	};

	template <class Sequence>
	struct deref_if_unary_result<Sequence, false> {
		typedef Sequence type;
	};


	struct deref_if_unary_impl {

		template <class Sequence>
		typename boost::enable_if<
			is_unary_vector<Sequence>,
			typename boost::add_reference<
				typename deref_if_unary_result<Sequence>::type
			>::type
		>::type
		operator()(Sequence& s) const {
			return boost::fusion::at_c<0>(s);
		}

		template <class Sequence>
		typename boost::disable_if<
			is_unary_vector<Sequence>,
			typename boost::add_reference<
				typename deref_if_unary_result<Sequence>::type
			>::type
		>::type
		operator()(Sequence& s) const {
			return s;
		}
	};

	// TODO: Rename this strange name.
	template <class Sequence>
	struct deref_if_unary {
		typedef typename deref_if_unary_result<Sequence>::type type;

		type operator()(Sequence& sequence) const {
			return deref_if_unary_impl()(sequence);
		}
	};

	template <class Sequence>
	typename deref_if_unary_result<Sequence>::type
	unary_deref(Sequence& sequence) {
		return deref_if_unary_impl()(sequence);
	}

}}}
