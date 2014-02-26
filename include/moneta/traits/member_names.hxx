#pragma once
#include "detail/member_trait_base.hxx"

MONETA_DECLARE_MEMBER_TRAIT(member_name, std::string)

#define MONETA_MEMBER_NAME(member, name) \
	MONETA_DEFINE_TRAIT_WITH_GET(member_name, member, std::string, #name)

namespace moneta { namespace traits {
	MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(member_name, get_member_names)
}}
