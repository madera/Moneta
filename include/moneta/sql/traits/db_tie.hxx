#pragma once
#include "../../traits/pk_tie_or_type.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename moneta::traits::detail::mpl::vector<EntityType>::type,
			moneta::traits::detail::pk_tie_or_type<boost::mpl::_1>
		>::type
	> {};

	template <class EntityType>
	struct const_db_tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename moneta::traits::detail::mpl::vector<EntityType>::type,
			moneta::traits::detail::const_pk_tie_or_type<boost::mpl::_1>
		>::type
	> {};

}}}
