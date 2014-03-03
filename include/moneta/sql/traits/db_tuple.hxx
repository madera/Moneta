#pragma once
#include "../../traits/tuple.hxx"
#include "../../traits/pk_or_type.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_tuple : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename moneta::traits::detail::mpl::vector<EntityType>::type,
			moneta::traits::detail::pk_or_type<boost::mpl::_1>
		>::type
	> {};

}}}
