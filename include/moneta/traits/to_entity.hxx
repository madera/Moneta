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
#include "to_tie.hxx"

namespace moneta { namespace traits {

	template <class Entity, class Tuple>
	Entity to_entity(const Tuple& tuple) {
		Entity entity;
		to_tie(entity) = tuple;
		return entity;
	}

}}
