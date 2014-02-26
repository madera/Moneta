#pragma once
#include "detail/member_trait_base.hxx"
#include "members.hxx"
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include <vector>
#include <string>

MONETA_DECLARE_MEMBER_TRAIT(member_name, std::string)

#define MONETA_MEMBER_NAME(member, name) \
	MONETA_DEFINE_MEMBER_TRAIT(member_name, member, std::string, name)

namespace moneta { namespace traits {

	MONETA_DEFINE_MEMBER_TRAIT_ACCESSOR(member_name, get_member_names)

}}
