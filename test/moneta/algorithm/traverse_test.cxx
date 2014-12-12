#include "stdafx.h"

#include <moneta/pp/sql_entity.hxx>
#include <string>

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Fingers;
};

//typedef moneta::entity_descriptor<
//	Person,
//	members<
//		member<Person, int,         ID     >,
//		member<Person, std::string, Name   >,
//		member<Person, double,      Height >,
//		member<Person, int,         Fingers>
//	>
//> descriptor;


//MONETA_DESCRIBE_SQL_ENTITY(Person, PERSON, ((int, ID, PERSON_ID))((std::string, Name, PERSON_NAME))((double, Height, PERSON_HEIGHT ))((int, Fingers, PERSON_FINGERS)))

#define M ((int, ID, PERSON_ID))((std::string, Name, PERSON_NAME))((double, Height, PERSON_HEIGHT ))((int, Fingers, PERSON_FINGERS))

MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(Person, M)
