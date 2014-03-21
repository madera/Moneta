#pragma once
#include "../../traits/pk_tie.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_pk_tie : boost::fusion::result_of::as_vector<
		typename moneta::traits::detail::mpl::pk_tie<EntityType>::type
	> {};

}}}
