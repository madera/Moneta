#pragma once
#include "../../traits/pk.hxx"
#include "../../traits/tuple.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_pk_tuple : moneta::traits::detail::deref_if_unary<
		typename boost::fusion::result_of::as_vector<
			typename boost::mpl::transform<
				typename moneta::traits::detail::mpl::pk<EntityType>::type,
				moneta::traits::pk<boost::mpl::_1>
			>::type
		>::type
	> {};

}}}
