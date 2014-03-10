//
// User entity.
//
#pragma once
#include <moneta/traits/pk.hxx>
#include <moneta/traits/members.hxx>
#include <moneta/traits/member.hxx>
#include <moneta/traits/member_names.hxx>
#include <moneta/sql/traits/table_name.hxx>
#include <moneta/sql/traits/field_names.hxx>
#include <boost/mpl/vector.hpp>
#include <string>
#include <moneta/pp/core.hxx>

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Fingers;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Person, PERSON,
	((int,         ID,      PERSON_ID,     MONETA_PRIMARY_KEY))
	((std::string, Name,    PERSON_NAME                      ))
	((double,      Height,  PERSON_HEIGHT                    ))
	((int,         Fingers, PERSON_FINGERS                   ))
)
