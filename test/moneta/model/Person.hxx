//
// User entity.
//
#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <string>

MONETA_DEFINE_AND_DESCRIBE_SQL_ENTITY(
	Person, PERSON,
	((int,         ID,      PERSON_ID,     MONETA_PRIMARY_KEY))
	((std::string, Name,    PERSON_NAME                      ))
	((double,      Height,  PERSON_HEIGHT                    ))
	((int,         Fingers, PERSON_FINGERS                   ))
)
