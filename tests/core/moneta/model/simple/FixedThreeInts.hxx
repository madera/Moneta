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
#include <moneta/pp/describe_entity.hxx>
#include <moneta/traits/fixed_values.hxx>

MONETA_DEFINE_ENTITY(
	FixedThreeInts,
	((int, One  ))
	((int, Two  ))
	((int, Three))
)

MONETA_FIXED_VALUE(MONETA_MEMBER(FixedThreeInts, int, One), 0x11)
MONETA_FIXED_VALUE(MONETA_MEMBER(FixedThreeInts, int, Two), 0x22)
