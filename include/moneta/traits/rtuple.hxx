#pragma once
#include "../traits/pk.hxx"
#include "../traits/tuple.hxx"

namespace moneta { namespace traits {

	template <class EntityType>
	struct rtuple : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename moneta::traits::detail::mpl::vector<EntityType>::type,
			moneta::traits::pk<boost::mpl::_1>
		>::type
	> {};

}}
