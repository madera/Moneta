#pragma once
#include "../../traits/detail/member_trait_base.hxx"

MONETA_DECLARE_MEMBER_TRAIT(sql_field_name, std::string)

#define MONETA_SQL_FIELD_NAME(member, field) \
	MONETA_DEFINE_MEMBER_TRAIT(sql_field_name, member, std::string, field)

namespace moneta { namespace sql { namespace traits {

	MONETA_DEFINE_MEMBER_TRAIT_ACCESSOR(sql_field_name, get_field_names)

}}}
