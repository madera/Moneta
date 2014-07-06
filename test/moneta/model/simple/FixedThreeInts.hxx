#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <moneta/traits/fixed_values.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	FixedThreeInts,
	((int, One  ))
	((int, Two  ))
	((int, Three))
)

MONETA_FIXED_VALUE(MONETA_MEMBER(FixedThreeInts, int, One), 0x11)
MONETA_FIXED_VALUE(MONETA_MEMBER(FixedThreeInts, int, Two), 0x22)