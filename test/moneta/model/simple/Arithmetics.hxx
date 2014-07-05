#pragma once
#include <moneta/pp/sql_entity.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Arithmetics,
	((bool,  Bool))
	((char,  Char))
	((short, Short))
	((int,   Int))
	((long,  Long))
)
