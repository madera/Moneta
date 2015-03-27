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
#include <moneta/sql/describe_sql_entity.hxx>
#include <string>

struct Address {
	int ID;
	int Number;
	std::string Street;
};

//((int,         ID,     ADDRESS_ID,    MONETA_PRIMARY_KEY))
MONETA_DESCRIBE_SQL_ENTITY(
	Address, ADDRESS,
	((int,         ID,     ADDRESS_ID    ))
	((int,         Number, ADDRESS_NUMBER))
	((std::string, Street, ADDRESS_STREET))
)

MONETA_DECLARE_PRIMARY_KEY(Address, int, ID)
