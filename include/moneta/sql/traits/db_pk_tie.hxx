#pragma once
#include "../../traits/pk_tie.hxx"

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_pk_tie : moneta::traits::detail::deref_if_unary<
		typename boost::fusion::result_of::as_vector<
			typename boost::mpl::transform<
				typename moneta::traits::detail::mpl::pk_tie<EntityType>::type,
				moneta::traits::pk_tie<boost::mpl::_1>
			>::type
		>::type
	> {};

}}}
