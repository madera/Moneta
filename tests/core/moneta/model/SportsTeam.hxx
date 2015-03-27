// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

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
