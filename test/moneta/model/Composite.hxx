//
// User entity.
//
#pragma once
#include "Person.hxx"
#include "Dog.hxx"

struct Composite {
	int Identifier;
	Person Person;
	Dog Dog;
};

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<Composite> : boost::mpl::vector<
		MONETA_MEMBER(Composite, int,    Identifier),
		MONETA_MEMBER(Composite, Person, Person),
		MONETA_MEMBER(Composite, Dog,    Dog   )
	> {};
}}}

MONETA_PRIMARY_KEY(MONETA_MEMBER(Composite, int, Identifier))

MONETA_MEMBER_NAME(MONETA_MEMBER(Composite, int,    Identifier), Identifier)
MONETA_MEMBER_NAME(MONETA_MEMBER(Composite, Person, Person    ), Person    )
MONETA_MEMBER_NAME(MONETA_MEMBER(Composite, Dog,    Dog       ), Dog       )

MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Composite, int,    Identifier), COMPOSITE_IDENTIFIER)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Composite, Person, Person    ), COMPOSITE_PERSON    )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Composite, Dog,    Dog       ), COMPOSITE_DOG       )
