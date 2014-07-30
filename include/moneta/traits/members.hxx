#pragma once
#include "../traits/detail/pure_type.hxx"
#include "detail/member_trait_base.hxx"

MONETA_DECLARE_ENTITY_TRAIT(members_of)

namespace moneta { namespace traits {

	template <class EntityType>
	struct members : detail::members_of<
		typename pure_type<EntityType>::type
	> {};

}}
