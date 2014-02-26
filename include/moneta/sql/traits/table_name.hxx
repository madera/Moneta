#pragma once
#include "../../traits/detail/member_trait_base.hxx"

MONETA_DECLARE_MEMBER_TRAIT(sql_table_name, std::string)

#define MONETA_SQL_TABLE_NAME(klass, field) \
	MONETA_DEFINE_MEMBER_TRAIT(sql_table_name, klass, std::string, field)

namespace moneta { namespace sql { namespace traits {

	template <class EntityType>
	struct table_name : moneta::traits::detail::sql_table_name<EntityType> {
	};

}}}
