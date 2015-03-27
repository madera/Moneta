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
#include "detail/member_trait_base.hxx"

MONETA_DECLARE_ENTITY_TRAIT(entity_name)

#define MONETA_ENTITY_NAME(klass, name) \
	MONETA_DEFINE_TRAIT_WITH_GET(entity_name, klass, std::string, BOOST_PP_STRINGIZE(name))

namespace moneta { namespace traits {

	template <class Entity>
	const typename entity_name<typename pure_type<Entity>::type>::trait_type
	get_entity_name() {
		return entity_name<typename pure_type<Entity>::type>::get();
	}

}}
