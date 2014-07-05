#pragma once
#include <moneta/pp/sql_entity.hxx>
#include "TwoInts.hxx"
#include "ThreeInts.hxx"
#include "FourInts.hxx"

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	CascadedInts,
	((TwoInts,   TwoInts  ))
	((ThreeInts, ThreeInts))
	((FourInts,  FourInts ))
)
