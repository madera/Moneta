#pragma once
#include "../../traits/replace_entities_with.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_tuple : boost::fusion::result_of::as_vector<
		typename moneta::traits::replace_entities_with_their_pk<
			typename moneta::traits::detail::mpl::vector<EntityType>::type
		>::type
	> {};

	// TODO: Const version.
}}}
