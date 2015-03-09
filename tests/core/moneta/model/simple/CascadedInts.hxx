#pragma once
#include <moneta/pp/describe_entity.hxx>
#include "TwoInts.hxx"
#include "ThreeInts.hxx"
#include "FourInts.hxx"

MONETA_DEFINE_ENTITY(
	CascadedInts,
	((::TwoInts,   TwoInts  ))
	((::FourInts,  FourInts ))
)
