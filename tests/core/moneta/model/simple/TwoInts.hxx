#pragma once
#include <moneta/pp/sql_entity.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	TwoInts,
	((int, One  ))
	((int, Two  ))
)
