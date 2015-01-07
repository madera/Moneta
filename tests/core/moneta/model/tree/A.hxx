#pragma once
#include <moneta/pp/describe_entity.hxx>

// Test tree:
//
//     A________
//    / \ \     \
//   f  g  B__   h
//        / \ \
//       C  i  D
//     / |    / \
//    j  k   l   E
//              / \
//             m   n
//

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	E,
	((int, m))
	((int, n))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	D,
	((int, l))
	((E  , e))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	C,
	((int, j))
	((int, k))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	B,
	((C,   c))
	((int, i))
	((D  , d))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	A,
	((int, f))
	((int, g))
	((B,   b))
	((int, h))
)
