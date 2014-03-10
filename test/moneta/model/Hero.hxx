//
// User entity.
//
#pragma once
#include "Cat.hxx"
#include "Dog.hxx"
#include "Address.hxx"
#include "Person.hxx"

MONETA_DEFINE_AND_DESCRIBE_SQL_ENTITY(
	Hero, HERO,
	((int,                      ID,         HERO_ID,        MONETA_PRIMARY_KEY))
	((std::string,              Name,       HERO_NAME                         ))
	((boost::optional<Cat>,     Cat,        HERO_CAT                          ))
	((boost::optional<Dog>,     Dog,        HERO_DOG                          ))
	((boost::optional<Address>, Address,    HERO_ADDRESS                      ))
	((boost::optional<Person>,  BestFriend, HERO_BESTFRIEND                   ))
	((boost::optional<Hero>,    Supervisor, HERO_SUPERVISOR                   ))
)
