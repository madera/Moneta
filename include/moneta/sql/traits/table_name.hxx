// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "../../traits/detail/member_trait_base.hxx"
#include <string>

MONETA_DECLARE_ENTITY_TRAIT(sql_table_name)

#define MONETA_SQL_TABLE_NAME(klass, field) \
	MONETA_DEFINE_TRAIT_WITH_GET(sql_table_name, klass, std::string, BOOST_PP_STRINGIZE(field))

namespace moneta { namespace sql { namespace traits {
	MONETA_DEFINE_ENTITY_TRAIT_GETTER(sql_table_name, get_table_name)
}}}
