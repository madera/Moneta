#pragma once
#include "members_of.hxx"
#include <vector>
#include <string>

namespace moneta { namespace meta {
	
	namespace detail {
		template <class EntityType>
		struct member_name_of {};
	}


	// MEQUEDE: Write member_names<ET>::get() based on get_field_names.

}}

#define MONETA_MEMBER_NAME(member, name) \
	__MONETA_MEMBER_TRAIT_BASE(moneta::meta::detail, member_name_of, member, const char*, name)
