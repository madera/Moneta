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
					meta::members_of<EntityType>::type,
					typename tie<EntityType>::type,
					EntityType&
				>()(entity);
			}

			typename const_tie<EntityType>::type
			operator()(const EntityType& entity) {
				return sequence_parameter_constructor_opfx<
					meta::members_of<EntityType>::type,
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
	typename traits::const_tie<EntityType>::type
	to_tie(const EntityType& x) {
		return detail::tie_maker<EntityType>()(x);
	}

	//template <class T, class U>
	//T to_entity(const U& tuple) {
	//	T x;
	//	to_tie(x) = tuple;
	//	return x;
	//}

	//template <class T, class U>
	//T to_entity_nc_dbg(U& tuple) {
	//	T x;
	//	to_tie(x) = tuple;
	//	return x;
	//}
}}
