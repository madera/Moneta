#pragma once
#include "../traits/detail/pure_type.hxx"
#include <boost/type_traits/integral_constant.hpp> // boost::false_type
#include "detail/member_trait_base.hxx"

MONETA_DECLARE_ENTITY_TRAIT(members_of)

namespace moneta { namespace traits {

	// TODO: Maybe this pattern will reappear. If it does, macro it off.
	template <class EntityType>
	struct members : detail::members_of<
		typename pure_type<EntityType>::type
	> {};

}}
