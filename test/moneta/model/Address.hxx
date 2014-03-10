//
// User entity.
//
#pragma once
#include <moneta/pp/sql_entity.hxx>
#include <string>

MONETA_DEFINE_AND_DESCRIBE_SQL_ENTITY(
	Address, ADDRESS,
	((int,         ID,     ADDRESS_ID,    MONETA_PRIMARY_KEY))
	((int,         Number, ADDRESS_NUMBER                   ))
	((std::string, Street, ADDRESS_STREET                   ))
)
