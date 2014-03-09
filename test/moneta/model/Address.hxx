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

struct Address {
	int ID;
	int Number;
	std::string Street;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Address, ADDRESS,
	((int,         ID,     ADDRESS_ID    ))
	((int,         Number, ADDRESS_NUMBER))
	((std::string, Street, ADDRESS_STREET))
)

MONETA_PRIMARY_KEY(MONETA_MEMBER(Address, int, ID))

//MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Address, int,         ID    ), ADDRESS_ID    )
//MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Address, int,         Number), ADDRESS_NUMBER)
//MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Address, std::string, Street), ADDRESS_STREET)
