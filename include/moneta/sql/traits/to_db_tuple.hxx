#pragma once
#include "db_tuple.hxx"
#include "../../traits/detail/sepacon_opfx.hxx"
#include "../../traits/is_entity.hxx"
#include "../../traits/tuple.hxx"
#include "../../traits/pk.hxx"
#include "../../traits/extract_pk.hxx"
#include "../../traits/detail/blanker.hxx"
#include <boost/utility/enable_if.hpp>

namespace moneta { namespace sql { namespace traits {

	namespace detail {

		template <class EntityType>
		struct db_tuple_maker {
			typename db_tuple<EntityType>::type
			operator()(EntityType& entity) {
				return moneta::traits::detail::sepacon_opfx<
					moneta::traits::members<EntityType>::type,
					typename db_tuple<EntityType>::type,
					EntityType&,
					moneta::traits::get_pk_functor
				>()(entity);
			}
		};

	}

	template <class EntityType>
	typename db_tuple<EntityType>::type
	to_db_tuple(EntityType& x) {
		return detail::db_tuple_maker<EntityType>()(x);
	}

	// XXX: Move this somewhere else.
	template <class EntityType>
	typename sql::traits::db_tuple<EntityType>::type make_db_tuple() {
		sql::traits::db_tuple<EntityType>::type result;
		return boost::fusion::transform(result, moneta::traits::detail::blanker());
	}

}}}
