#pragma once
#include "../traits/pk_tie.hxx"

namespace moneta { namespace traits {

	template <class Entity>
	struct rtie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename moneta::traits::detail::mpl::vector<Entity>::type,
			moneta::traits::pk_tie<boost::mpl::_1>
		>::type
	> {};

}}
