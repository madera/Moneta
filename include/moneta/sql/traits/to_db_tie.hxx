#pragma once
#include "db_tuple.hxx"
#include "../../traits/detail/sepacon_opfx.hxx"
#include "../../traits/is_entity.hxx"
#include "../../traits/tuple.hxx"
#include "../../traits/pk_or_type.hxx"
#include "../../traits/extract_pk.hxx"
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
					moneta::traits::pk_or_type_filter
				>()(entity);
			}
		};

	}

	template <class EntityType>
	typename db_tuple<EntityType>::type
	to_db_tuple(EntityType& x) {
		return detail::db_tuple_maker<EntityType>()(x);
	}

}}}
