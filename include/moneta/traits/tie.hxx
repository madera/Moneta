#pragma once
#include "tuple.hxx"

namespace moneta { namespace traits {

	template <typename EntityType>
	struct tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename detail::mpl::vector<EntityType>::type,
			boost::add_reference<boost::mpl::_1>
		>::type
	> {};

	template <typename EntityType>
	struct const_tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename detail::mpl::vector<EntityType>::type,
			boost::add_reference<boost::add_const<boost::mpl::_1> >
		>::type
	> {};

}}
