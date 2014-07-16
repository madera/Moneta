#pragma once
#include "traits/tuple.hxx"
#include "traits/to_tie.hxx"
#include "traits/detail/blanker.hxx"
#include "traits/fixed_values.hxx"
#include "traits/members.hxx"
#include "codec/for_each_member.hxx"
#include <boost/fusion/algorithm/transformation/transform.hpp>

namespace moneta {

	namespace detail {
		struct fixed_value_setter {
			template <class EntityType, class Member, class Path>
			void operator()(EntityType& entity, Member& member) const {
				member(entity) = traits::detail::fixed_value<Member>::get();
			}
		};
	}

	template <class EntityType>
	typename traits::tuple<EntityType>::type make_tuple() {
		traits::tuple<EntityType>::type result;

		// TODO: Add fixed_value assignment here.

		return boost::fusion::transform(result, traits::detail::blanker());
	}

	template <class EntityType>
	EntityType make_entity() {
		EntityType result;
		traits::to_tie<EntityType>(result) = make_tuple<EntityType>();

		moneta::codec::for_some_members<
			typename traits::fixed_value_members<EntityType>::type
		>(result, detail::fixed_value_setter());
		
		return result;
	}

}
