#pragma once
#include "../../traits/pk.hxx"
#include "../../traits/tuple.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_pk_tuple : boost::fusion::result_of::as_vector<
		typename moneta::traits::detail::mpl::pk<EntityType>::type
	> {};

}}}
