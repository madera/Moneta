//
// User entity.
//
#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <string>

struct Address {
	int ID;
	int Number;
	std::string Street;
};

//((int,         ID,     ADDRESS_ID,    MONETA_PRIMARY_KEY))
MONETA_DESCRIBE_SQL_ENTITY(
	Address, ADDRESS,
	((int,         ID,     ADDRESS_ID    ))
	((int,         Number, ADDRESS_NUMBER))
	((std::string, Street, ADDRESS_STREET))
)

MONETA_DECLARE_PRIMARY_KEY(Address, int, ID)
