#pragma once
#include "tuple.hxx"
#include "detail/sequence_parameter_constructor_opfx.hxx"

namespace moneta { namespace traits {

	namespace detail {
		template <class EntityType>
		struct tie_maker {
			typename tie<EntityType>::type
			operator()(EntityType& entity) {
				return sequence_parameter_constructor_opfx<
					members<EntityType>::type,
					typename tie<EntityType>::type,
					EntityType&
				>()(entity);
			}

			typename const_tie<EntityType>::type
			operator()(const EntityType& entity) {
				return sequence_parameter_constructor_opfx<
					members<EntityType>::type,
					typename const_tie<EntityType>::type,
					const EntityType&
				>()(entity);
			}
		};
	}

	template <class EntityType>
	typename tie<EntityType>::type
	to_tie(EntityType& x) {
		return detail::tie_maker<EntityType>()(x);
	}

	template <typename EntityType>
	typename const_tie<EntityType>::type
	to_tie(const EntityType& x) {
		return detail::tie_maker<EntityType>()(x);
	}

}}
