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
