#include "stdafx.h"
#include <moneta/pp/describe_entity.hxx>
#include <moneta/traits/fixed_values.hxx>

typedef char char_16[16];

MONETA_DEFINE_ENTITY_STRUCT_WITH_NAMESPACE(
	(ns0)(ns1)(ns2), some_entity,
	((int,     a))
	((double,  b))
	((char,    c))
	((char_16, d))
)

MONETA_FIXED_VALUE_MEMBER(ns0::ns1::ns2::some_entity, int,    a, 123)
MONETA_FIXED_VALUE_MEMBER(ns0::ns1::ns2::some_entity, double, b, 1.5)

// TODO: FIXME: This test lost focus.
