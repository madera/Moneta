//
// User entity.
//
#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <string>

MONETA_DEFINE_AND_DESCRIBE_SQL_ENTITY(
	Dog, DOG,
	((std::string, Owner, DOG_OWNER, MONETA_PRIMARY_KEY))
	((int,         ID,    DOG_ID,    MONETA_PRIMARY_KEY))
	((std::string, Name,  DOG_NAME                     ))
)
