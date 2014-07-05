#pragma once
#include "detail/member_trait_base.hxx"
#include "detail/has_member_trait.hxx"

MONETA_DECLARE_TRAIT(fixed_value)

#define MONETA_FIXED_VALUE(member, value) \
	MONETA_DEFINE_TRAIT_WITH_GET(fixed_value, member, member::result_type, value)
