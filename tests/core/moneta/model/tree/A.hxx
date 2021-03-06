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

// Test tree:
//
//     A__________
//    / |  |     |
//   f  g  B____ h
//        / |  |
//       C  i  D__
//     / |    /  |
//    j  k   l   E__
//              /  |
//             m   n
//

MONETA_DEFINE_ENTITY(
	E,
	((int, m))
	((int, n))
)

MONETA_DEFINE_ENTITY(
	D,
	((int, l))
	((E  , e))
)

MONETA_DEFINE_ENTITY(
	C,
	((int, j))
	((int, k))
)

MONETA_DEFINE_ENTITY(
	B,
	((C,   c))
	((int, i))
	((D  , d))
)

MONETA_DEFINE_ENTITY(
	A,
	((int, f))
	((int, g))
	((B,   b))
	((int, h))
)
