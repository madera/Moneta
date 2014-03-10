//
// User entity.
//
#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <string>

struct Dog {
	std::string Owner;
	int ID;
	std::string Name;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Dog, DOG,
	((std::string, Owner, DOG_OWNER, MONETA_PRIMARY_KEY))
	((int,         ID,    DOG_ID,    MONETA_PRIMARY_KEY))
	((std::string, Name,  DOG_NAME                     ))
)
