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

MONETA_PRIMARY_KEY(MONETA_MEMBER(Cat, int, ID))

MONETA_DESCRIBE_SQL_ENTITY(
	Cat, CAT,
	(int,         ID     , CAT_ID     )
	(std::string, Name   , CAT_NAME   )
	(Address,     Address, CAT_ADDRESS)
)
