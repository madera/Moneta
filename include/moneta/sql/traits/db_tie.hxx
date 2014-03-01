#pragma once
#include "db_tuple.hxx"


namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct db_tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename boost::fusion::result_of::as_vector<
				typename moneta::traits::replace_entities_with_pks<
					typename moneta::traits::detail::mpl::vector<EntityType>::type
				>::type
			>::type,
			boost::add_reference<boost::mpl::_1>
		>::type
	> {};

	template <class EntityType>
	struct const_db_tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename boost::fusion::result_of::as_vector<
				typename moneta::traits::replace_entities_with_pks<
					typename moneta::traits::detail::mpl::vector<EntityType>::type
				>::type
			>::type,
			boost::add_reference<boost::add_const<boost::mpl::_1> >
		>::type
	> {};

}}}
