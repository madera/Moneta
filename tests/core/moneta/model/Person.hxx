//
// User entity.
//
#pragma once
#include <moneta/sql/describe_sql_entity.hxx>
#include <string>

template <class T, class U, class V>
struct circle {
};

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Fingers;
//	circle<char, short, int> Circle;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Person, PERSON,
	((int,         ID,      PERSON_ID     ))
	((std::string, Name,    PERSON_NAME   ))
	((double,      Height,  PERSON_HEIGHT ))
	((int,         Fingers, PERSON_FINGERS))
)

MONETA_DECLARE_PRIMARY_KEY(Person, int, ID)

// Alternate non-variadic version:

//MONETA_DESCRIBE_SQL_ENTITY(
//	Person, PERSON,
//	((int        )(ID     )(PERSON_ID     ))
//	((std::string)(Name   )(PERSON_NAME   ))
//	((double     )(Height )(PERSON_HEIGHT ))
//	((int        )(Fingers)(PERSON_FINGERS))
//)
