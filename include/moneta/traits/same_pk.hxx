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
#include "pk.hxx"

namespace moneta { namespace traits {

	// FIXME: Do we really need this? Isn't vector's == enough?
	template <class Entity>
	const bool same_pk(const Entity& a, const Entity& b) {
		return extract_pk(a) == extract_pk(b);
	}

}}
