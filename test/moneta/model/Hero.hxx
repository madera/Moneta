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

MONETA_PRIMARY_KEY(MONETA_MEMBER(Hero, int, ID))

MONETA_DESCRIBE_SQL_ENTITY(
	Hero, HERO,
	(int,                      ID,         HERO_ID        )
	(std::string,              Name,       HERO_NAME      )
	(boost::optional<Cat>,     Cat,        HERO_CAT       )
	(boost::optional<Dog>,     Dog,        HERO_DOG       )
	(boost::optional<Address>, Address,    HERO_ADDRESS   )
	(boost::optional<Person>,  BestFriend, HERO_BESTFRIEND)
	(boost::optional<Hero>,    Supervisor, HERO_SUPERVISOR)
)
