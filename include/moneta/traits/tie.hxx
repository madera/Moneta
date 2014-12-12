#pragma once
#include "tuple.hxx"

namespace moneta { namespace traits {

	template <typename Entity>
	struct tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename detail::mpl::vector<Entity>::type,
			boost::add_reference<boost::mpl::_1>
		>::type
	> {};

}}
