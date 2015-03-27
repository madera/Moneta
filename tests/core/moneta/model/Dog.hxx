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

struct Dog {
	std::string Owner;
	int ID;
	std::string Name;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Dog, DOG,
	((std::string, Owner, DOG_OWNER))
	((int,         ID,    DOG_ID   ))
	((std::string, Name,  DOG_NAME ))
)

MONETA_DECLARE_PRIMARY_KEY(Dog, std::string, Owner)
MONETA_DECLARE_PRIMARY_KEY(Dog, int        , ID   )
