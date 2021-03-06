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
#include "../../traits/members.hxx"
#include "../../traits/pk.hxx"
#include <string>

MONETA_DECLARE_MEMBER_TRAIT(sql_field_name)

#define MONETA_SQL_FIELD_NAME(member, field) \
	MONETA_DEFINE_TRAIT_WITH_GET(sql_field_name, member, std::string, BOOST_PP_STRINGIZE(field))

namespace moneta { namespace sql { namespace traits {
	MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(sql_field_name, std::string, get_field_names)

	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(
		sql_field_name,
		std::string,
		get_pk_field_names,
		typename moneta::traits::pk_members<Entity>::type
	)
}}}
