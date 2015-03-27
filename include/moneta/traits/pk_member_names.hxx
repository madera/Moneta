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
#include "member_name.hxx"
#include "pk.hxx"

namespace moneta { namespace traits {

	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(
		member_name,
		std::string,
		get_pk_member_names, 
		typename pk_members<Entity>::type
	)

}}
