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
#include "Person.hxx"
#include "Dog.hxx"

struct Composite {
	int Identifier;
	::Person Person;
	::Dog Dog;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Composite, COMPOSITE,
	((int,    Identifier, COMPOSITE_IDENTIFIER))
	((Person, Person,     COMPOSITE_PERSON    ))
	((Dog,    Dog,        COMPOSITE_DOG       ))
)

MONETA_DECLARE_PRIMARY_KEY(Composite, int, Identifier)
