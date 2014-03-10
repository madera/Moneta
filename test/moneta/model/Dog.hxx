//
// User entity.
//
#pragma once
#include <moneta/traits/members.hxx>
#include <moneta/traits/member.hxx>
#include <moneta/traits/member_names.hxx>
#include <moneta/sql/traits/table_name.hxx>
#include <moneta/sql/traits/field_names.hxx>
#include <moneta/traits/pk.hxx>
#include <boost/mpl/vector.hpp>
#include <string>
#include <moneta/pp/core.hxx>

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
