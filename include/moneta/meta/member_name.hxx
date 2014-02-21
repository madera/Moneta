#pragma once
#include "detail\member_trait_base.hxx"

__MONETA_DECLARE_MEMBER_TRAIT(member_name)

#define MONETA_MEMBER_NAME(member, name) \
	__MONETA_MEMBER_TRAIT_BASE(moneta::meta::detail, member_name, member, const char*, name)
