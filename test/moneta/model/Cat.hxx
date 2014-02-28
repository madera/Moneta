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

#include "Address.hxx"

struct Cat {
	int ID;
	std::string Name;
	Address Address;
};

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<Cat> : boost::mpl::vector<
		MONETA_MEMBER(Cat, int,         ID     ),
		MONETA_MEMBER(Cat, std::string, Name   ),
		MONETA_MEMBER(Cat, Address,     Address)
	> {};
}}}

MONETA_PRIMARY_KEY(MONETA_MEMBER(Cat, int, ID))

MONETA_MEMBER_NAME(MONETA_MEMBER(Cat, int,         ID     ), ID     )
MONETA_MEMBER_NAME(MONETA_MEMBER(Cat, std::string, Name   ), Name   )
MONETA_MEMBER_NAME(MONETA_MEMBER(Cat, Address,     Address), Address)

MONETA_SQL_TABLE_NAME(Cat, CAT)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Cat, int,         ID     ), CAT_ID     )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Cat, std::string, Name   ), CAT_NAME   )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Cat, Address,     Address), CAT_ADDRESS)
