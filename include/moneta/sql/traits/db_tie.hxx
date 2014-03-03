#pragma once
#include "db_tuple.hxx"


namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_tie : boost::fusion::result_of::as_vector<
		typename moneta::traits::replace_entities_with_pk_ties<
			typename moneta::traits::detail::mpl::vector<EntityType>::type
		>::type
	> {};

	//template <class EntityType>
	//struct const_db_tie : boost::fusion::result_of::as_vector<
	//	// TBD
	//> {};

}}}
