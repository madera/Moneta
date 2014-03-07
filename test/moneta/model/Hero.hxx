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
#include <boost/optional.hpp>
#include <string>

#include "Cat.hxx"
#include "Dog.hxx"
#include "Address.hxx"
#include "Person.hxx"

struct Hero {
	int ID;
	std::string Name;
	boost::optional<Cat> Cat;
	boost::optional<Dog> Dog;
	boost::optional<Address> Address;
	boost::optional<Person> BestFriend;
	boost::optional<Hero> Supervisor;
};

namespace moneta {
	namespace traits {
		namespace detail {
			template <>
			struct members_of<Hero> : boost::mpl::vector<
				MONETA_MEMBER(Hero, int, ID),
				MONETA_MEMBER(Hero, std::string, Name),
				MONETA_MEMBER(Hero, boost::optional<Cat>,     Cat),
				MONETA_MEMBER(Hero, boost::optional<Dog>,     Dog),
				MONETA_MEMBER(Hero, boost::optional<Address>, Address),
				MONETA_MEMBER(Hero, boost::optional<Person>,  BestFriend),
				MONETA_MEMBER(Hero, boost::optional<Hero>,    Supervisor)
			>{};
		}
	}
}

MONETA_PRIMARY_KEY(MONETA_MEMBER(Hero, int, ID))

MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, int,                      ID        ), ID        )
MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, std::string,              Name      ), Name      )
MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, boost::optional<Cat>,     Cat       ), Cat       )
MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, boost::optional<Dog>,     Dog       ), Dog       )
MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, boost::optional<Address>, Address   ), Address   )
MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, boost::optional<Person>,  BestFriend), BestFriend)
MONETA_MEMBER_NAME(MONETA_MEMBER(Hero, boost::optional<Hero>,    Supervisor), Supervisor)

MONETA_SQL_TABLE_NAME(Hero, HERO)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, int,                      ID        ), HERO_ID        )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, std::string,              Name      ), HERO_NAME      )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, boost::optional<Cat>,     Cat       ), HERO_CAT       )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, boost::optional<Dog>,     Dog       ), HERO_DOG       )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, boost::optional<Address>, Address   ), HERO_ADDRESS   )
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, boost::optional<Person>,  BestFriend), HERO_BESTFRIEND)
MONETA_SQL_FIELD_NAME(MONETA_MEMBER(Hero, boost::optional<Hero>,    Supervisor), HERO_SUPERVISOR)
