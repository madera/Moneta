#pragma once
#include "db_tie.hxx"
#include "../../traits/detail/sepacon_opfx.hxx"
#include "../../traits/is_entity.hxx"
#include "../../traits/tie.hxx"
#include "../../traits/pk.hxx"
#include "../../traits/extract_pk.hxx"
#include <boost/utility/enable_if.hpp>

namespace moneta { namespace sql { namespace traits {

	namespace detail {

		template <class EntityType>
		struct db_tie_maker {
			typename db_tie<EntityType>::type
			operator()(EntityType& entity) {
				return moneta::traits::detail::sepacon_opfx<
					moneta::traits::members<EntityType>::type,
					typename db_tie<EntityType>::type,
					EntityType&,
					moneta::traits::get_pk_tie_functor
				>()(entity);
			}
		};

	}

	template <class EntityType>
	typename db_tie<EntityType>::type
	to_db_tie(EntityType& x) {
		return detail::db_tie_maker<EntityType>()(x);
	}

}}}
