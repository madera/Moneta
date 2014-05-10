//
// User entity.
//
#pragma once
#include "Address.hxx"

struct Cat {
	int ID;
	std::string Name;
	Address Address;
};

//((int,         ID,      CAT_ID,     MONETA_PRIMARY_KEY))
MONETA_DESCRIBE_SQL_ENTITY(
	Cat, CAT,
	((int,         ID,      CAT_ID     ))
	((std::string, Name,    CAT_NAME   ))
	((Address,     Address, CAT_ADDRESS))
)

MONETA_DECLARE_PRIMARY_KEY(Cat, int, ID)
