//
// User entity.
//
#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <string>
#include "Person.hxx"

struct Lizard {
	Person Owner;
	int ID;
	std::string Name;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Lizard, LIZARD,
	((Person,      Owner, LIZARD_OWNER, MONETA_PRIMARY_KEY))
	((int,         ID,    LIZARD_ID,    MONETA_PRIMARY_KEY))
	((std::string, Name,  LIZARD_NAME                     ))
)
