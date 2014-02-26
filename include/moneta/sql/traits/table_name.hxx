#pragma once
#include "../../traits/detail/member_trait_base.hxx"

MONETA_DECLARE_ENTITY_TRAIT(sql_table_name, std::string)

#define MONETA_SQL_TABLE_NAME(klass, field) \
	MONETA_DEFINE_TRAIT_WITH_GET(sql_table_name, klass, std::string, #field)

namespace moneta { namespace sql { namespace traits {
	MONETA_DEFINE_ENTITY_TRAIT_GETTER(sql_table_name, get_table_name)
}}}
