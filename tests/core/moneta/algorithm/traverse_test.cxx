// XXX: Something went wrong here: Where's the code?

#include "stdafx.h"

#include <moneta/sql/describe_sql_entity.hxx>
#include <string>

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Fingers;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Person, PERSON,
	((int,         ID,      PERSON_ID     ))
	((std::string, Name,    PERSON_NAME   ))
	((double,      Height,  PERSON_HEIGHT ))
	((int,         Fingers, PERSON_FINGERS))
)

MONETA_DECLARE_PRIMARY_KEY(Person, int, ID)
