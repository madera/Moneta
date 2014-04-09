#pragma once
#include "rtie.hxx"
#include "../traits/detail/sepacon_opfx.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/tie.hxx"
#include "../traits/pk.hxx"
#include "../traits/extract_pk.hxx"
#include <boost/utility/enable_if.hpp>

namespace moneta { namespace sql { namespace traits {

	namespace detail {

		template <class EntityType>
		struct rtie_maker {
			typename rtie<EntityType>::type
			operator()(EntityType& entity) {
				return moneta::traits::detail::sepacon_opfx<
					moneta::traits::members<EntityType>::type,
					typename rtie<EntityType>::type,
					EntityType&,
					moneta::traits::get_pk_tie_functor
				>()(entity);
			}
		};

	}

	template <class EntityType>
	typename rtie<EntityType>::type
	to_rtie(EntityType& x) {
		return detail::rtie_maker<EntityType>()(x);
	}

}}}
