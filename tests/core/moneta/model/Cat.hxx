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

//
// User entity.
//
#pragma once
#include "Address.hxx"

struct Cat {
	int ID;
	std::string Name;
	::Address Address;
};

//((int,         ID,      CAT_ID,     MONETA_PRIMARY_KEY))
MONETA_DESCRIBE_SQL_ENTITY(
	Cat, CAT,
	((int,         ID,      CAT_ID     ))
	((std::string, Name,    CAT_NAME   ))
	((Address,     Address, CAT_ADDRESS))
)

MONETA_DECLARE_PRIMARY_KEY(Cat, int, ID)
