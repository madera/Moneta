#pragma once
#include "is_unary_vector.hxx"
#include <boost/fusion/view/transform_view.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/identity.hpp>

namespace moneta { namespace traits { namespace detail {

	struct deref_if_unary_impl {

		template <class Sequence, bool Unary = is_unary_vector<Sequence>::value>
		struct derefed_result : boost::mpl::identity<Sequence> {};

		template <class Sequence>
		struct derefed_result<Sequence, true> : boost::mpl::identity<
			typename boost::mpl::at_c<Sequence, 0>::type
		> {};

		template <class Sequence>
		typename boost::enable_if<
			is_unary_vector<Sequence>,
			typename boost::add_reference<
				typename derefed_result<Sequence>::type
			>::type
		>::type
		operator()(Sequence& s) const {
			return boost::fusion::at_c<0>(s);
		}

		template <class Sequence>
		typename boost::disable_if<
			is_unary_vector<Sequence>,
			typename boost::add_reference<
				typename derefed_result<Sequence>::type
			>::type
		>::type
		operator()(Sequence& s) const {
			return s;
		}

	};

	template <class Sequence>
	typename deref_if_unary_impl::derefed_result<Sequence>::type
	deref_unary_vector(Sequence& sequence) {
		return deref_if_unary_impl()(sequence);
	}

	// TODO: Rename this strange name. Or maybe just delete it.
	template <class Sequence>
	struct deref_if_unary {
		typedef typename deref_if_unary_impl::derefed_result<Sequence>::type type;
		type operator()(Sequence& sequence) const {
			return deref_if_unary_impl()(sequence);
		}
	};

}}}
