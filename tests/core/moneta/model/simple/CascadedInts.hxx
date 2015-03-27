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
#include "TwoInts.hxx"
#include "ThreeInts.hxx"
#include "FourInts.hxx"

MONETA_DEFINE_ENTITY(
	CascadedInts,
	((::TwoInts,   TwoInts  ))
	((::FourInts,  FourInts ))
)
