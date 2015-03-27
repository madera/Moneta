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
#include "Person.hxx"

struct Lizard {
	Person Owner;
	int ID;
	std::string Name;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Lizard, LIZARD,
	((Person,      Owner, LIZARD_OWNER))
	((int,         ID,    LIZARD_ID   ))
	((std::string, Name,  LIZARD_NAME ))
)

MONETA_DECLARE_PRIMARY_KEY(Lizard, Person, Owner)
MONETA_DECLARE_PRIMARY_KEY(Lizard, int   , ID   )
