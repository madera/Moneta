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
#include <moneta/pp/describe_entity.hxx>
#include <boost/optional.hpp>
#include "Address.hxx"
#include "Dog.hxx"

MONETA_DEFINE_ENTITY(
	Customer,
	((std::string,                                Name       ))
	((time_t,                                     DOB        ))
	((boost::optional<Address>,                   HomeAddress))
	((boost::optional<Address>,                   WorkAddress))
	((boost::optional<int>,                       Rating     ))
	((boost::optional<std::vector<Dog> >,         Dogs       ))
	((boost::optional<std::vector<std::string> >, Tags       ))
)
