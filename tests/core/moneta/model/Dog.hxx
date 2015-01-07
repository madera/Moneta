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
