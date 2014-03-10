//
// User entity.
//
#pragma once
#include "Address.hxx"

MONETA_DEFINE_AND_DESCRIBE_SQL_ENTITY(
	Cat, CAT,
	((int,         ID,      CAT_ID,     MONETA_PRIMARY_KEY))
	((std::string, Name,    CAT_NAME                      ))
	((Address,     Address, CAT_ADDRESS                   ))
)
