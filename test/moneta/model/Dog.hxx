//
// User entity.
//
#include <moneta/traits/members.hxx>
#include <moneta/traits/member.hxx>
#include <moneta/traits/member_names.hxx>
#include <moneta/sql/traits/table_name.hxx>
#include <moneta/sql/traits/field_names.hxx>
#include <moneta/traits/pk.hxx>
#include <boost/mpl/vector.hpp>
#include <string>

struct Dog {
	std::string Owner;
	int ID;
	std::string Name;
};

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<Dog> : boost::mpl::vector<
		MONETA_MEMBER(Dog, std::string, Owner),
		MONETA_MEMBER(Dog, int,         ID   ),
		MONETA_MEMBER(Dog, std::string, Name )
	> {};
}}}

MONETA_PRIMARY_KEY(MONETA_MEMBER(Dog, std::string, Owner))
MONETA_PRIMARY_KEY(MONETA_MEMBER(Dog, int,         ID   ))

MONETA_MEMBER_NAME(MONETA_MEMBER(Dog, std::string, Owner  ), Owner  )
MONETA_MEMBER_NAME(MONETA_MEMBER(Dog, int,         ID     ), ID     )
MONETA_MEMBER_NAME(MONETA_MEMBER(Dog, std::string, Name   ), Name   )


MONETA_SQL_TABLE_NAME(Dog, DOG)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Dog, std::string, Owner  ), DOG_OWNER)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Dog, int,         ID     ), DOG_ID   )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Dog, std::string, Name   ), DOG_NAME )
