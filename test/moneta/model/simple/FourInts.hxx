#pragma once
#include <moneta/pp/sql_entity.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	FourInts,
	((int, One  ))
	((int, Two  ))
	((int, Three))
	((int, Four ))
)
