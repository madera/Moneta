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

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Ratings;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Person, PERSON,
	((int,         ID,      PERSON_ID     ))
	((std::string, Name,    PERSON_NAME   ))
	((double,      Height,  PERSON_HEIGHT ))
	((int,         Ratings, PERSON_RATINGS))
)

MONETA_DECLARE_PRIMARY_KEY(Person, int, ID)

// -----------------------------------------------

// TODO: Finish these alternative versions...

// Alternate non-variadic version:

//MONETA_DESCRIBE_SQL_ENTITY(
//	Person, PERSON,
//	((int        )(ID     )(PERSON_ID     ))
//	((std::string)(Name   )(PERSON_NAME   ))
//	((double     )(Height )(PERSON_HEIGHT ))
//	((int        )(Ratings)(PERSON_RATINGS))
//)
