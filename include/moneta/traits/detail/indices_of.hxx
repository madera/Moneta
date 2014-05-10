#pragma once
#include <boost/mpl/print.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/pair.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Sequence, class Predicate>
	struct indices_of : boost::mpl::second<
		typename boost::mpl::fold<
			Sequence,
			boost::mpl::pair<
				boost::mpl::int_<0>,
				boost::mpl::vector0<>
			>,
			boost::mpl::pair<
				boost::mpl::next<
					boost::mpl::first<boost::mpl::_1>
				>,
				boost::mpl::if_<
					boost::mpl::bind<
						typename boost::mpl::lambda<Predicate>::type,
						boost::mpl::_2
					>,
					boost::mpl::push_back<
						boost::mpl::second<boost::mpl::_1>,
						boost::mpl::first<boost::mpl::_1>
					>,
					boost::mpl::second<boost::mpl::_1>
				>
			>
		>::type
	> {};

}}}