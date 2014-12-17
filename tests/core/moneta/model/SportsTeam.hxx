#pragma once
#include "Person.hxx"

struct SportsTeam {
	std::string Name;
	std::vector<Person> Players;
	std::vector<std::string> Tags;
};

MONETA_DESCRIBE_SQL_ENTITY(
	SportsTeam, SPORTS_TEAM,
	((std::string,              Name,    SPORTS_TEAM_NAME   ))
	((std::vector<Person>,      Players, SPORTS_TEAM_PLAYERS))
	((std::vector<std::string>, Tags,    SPORTS_TEAM_TAGS   ))
)
