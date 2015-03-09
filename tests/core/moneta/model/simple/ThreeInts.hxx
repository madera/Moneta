#pragma once
#include <moneta/pp/describe_entity.hxx>
#include <moneta/traits/fixed_values.hxx>

MONETA_DEFINE_ENTITY(
	ThreeInts,
	((int, One  ))
	((int, Two  ))
	((int, Three))
)

MONETA_FIXED_VALUE(MONETA_MEMBER(ThreeInts, int, One), 0x11111111)
MONETA_FIXED_VALUE(MONETA_MEMBER(ThreeInts, int, Two), 0x22222222)
