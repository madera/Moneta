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

// -----
#define ENTITY_DEFINITION \
	FQN((Model)(Address)) \
	MEMBERS_BEGIN \
		SQL_MEMBER(int,         ID,     ADDRESS_ID    ) \
		SQL_MEMBER(int,         Number, ADDRESS_NUMBER) \
		SQL_MEMBER(std::string, Street, ADDRESS_STREET) \
	MEMBERS_END

#define MONETA_ENTITY_PROCESSOR <moneta/pp/specs/struct_spec.hxx>
#include <moneta/end_entity>
// -----

struct Address {
	int ID;
	int Number;
	std::string Street;
};

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<Address> : boost::mpl::vector<
		MONETA_MEMBER(Address, int,         ID    ),
		MONETA_MEMBER(Address, int,         Number),
		MONETA_MEMBER(Address, std::string, Street)
	> {};
}}}

MONETA_PRIMARY_KEY(MONETA_MEMBER(Address, int, ID))

MONETA_MEMBER_NAME(MONETA_MEMBER(Address, int,         ID    ), ID    )
MONETA_MEMBER_NAME(MONETA_MEMBER(Address, int,         Number), Number)
MONETA_MEMBER_NAME(MONETA_MEMBER(Address, std::string, Street), Street)

MONETA_SQL_TABLE_NAME(Address, ADDRESS)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Address, int,         ID    ), ADDRESS_ID    )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Address, int,         Number), ADDRESS_NUMBER)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Address, std::string, Street), ADDRESS_STREET)
