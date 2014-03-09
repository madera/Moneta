//
// User entity.
//
#pragma once
#include "Person.hxx"
#include "Dog.hxx"

struct Composite {
	int Identifier;
	Person Person;
	Dog Dog;
};

MONETA_PRIMARY_KEY(MONETA_MEMBER(Composite, int, Identifier))

MONETA_DESCRIBE_SQL_ENTITY(
	Composite, COMPOSITE,
	(int,    Identifier, COMPOSITE_IDENTIFIER)
	(Person, Person,     COMPOSITE_PERSON    )
	(Dog,    Dog,        COMPOSITE_DOG       )
)
