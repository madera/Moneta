#pragma once
#include "detail\member_trait_base.hxx"

MONETA_DECLARE_MEMBER_TRAIT(member_name, std::string)

#define MONETA_MEMBER_NAME(member, name) MONETA_MEMBER_TRAIT_BASE(member_name, member, std::string, name)
