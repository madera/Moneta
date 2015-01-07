// TODO: FIXME: This needs serious work on the repetition and/or dispatch logic!!!

#pragma once
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Sequence>
	struct basic_tie_vector : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform_view<
			Sequence,
			boost::add_reference<boost::mpl::_1>
		>::type
	> {};

	template <class Sequence, int Size = boost::mpl::size<Sequence>::value>
	struct tie_vector_impl;

	// TODO: Boost.PPize this.

	template <class Sequence>
	struct tie_vector_impl<Sequence, 1> : basic_tie_vector<Sequence> {
		typename basic_tie_vector<Sequence>::type operator()(Sequence& s) {
			using boost::fusion::at_c;
			return typename basic_tie_vector<Sequence>::type(at_c<0>(s));
		}
	};

	template <class Sequence>
	struct tie_vector_impl<Sequence, 2> : basic_tie_vector<Sequence> {
		typename basic_tie_vector<Sequence>::type operator()(Sequence& s) {
			using boost::fusion::at_c;
			return typename basic_tie_vector<Sequence>::type(at_c<0>(s), at_c<1>(s));
		}
	};

	template <class Sequence>
	struct tie_vector_impl<Sequence, 3> : basic_tie_vector<Sequence> {
		typename basic_tie_vector<Sequence>::type operator()(Sequence& s) {
			using boost::fusion::at_c;
			return typename basic_tie_vector<Sequence>::type(at_c<0>(s), at_c<1>(s), at_c<2>(s));
		}
	};

	template <class Sequence>
	typename tie_vector_impl<Sequence>::type
	tie_vector(Sequence& sequence) {
		return tie_vector_impl<Sequence>()(sequence);
	}

}}}
