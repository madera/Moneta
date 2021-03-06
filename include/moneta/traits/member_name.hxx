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

// TODO: Rename this file.

#pragma once
#include "members.hxx"
#include <boost/assert.hpp>

#include <boost/preprocessor/stringize.hpp>

MONETA_DECLARE_MEMBER_TRAIT(member_name)

#define MONETA_MEMBER_NAME(member, name) \
	MONETA_DEFINE_TRAIT_WITH_GET(member_name, (member), std::string, BOOST_PP_STRINGIZE(name))

namespace moneta { namespace traits {

	MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(member_name, std::string, get_member_names)

	// FIXME: O(n): Sloooowwww.
	// XXX: Optimize this using hashes or something...
	template <class Entity>
	size_t get_member_name_index(const std::string& name) {
		const std::vector<std::string>& data = get_member_names<Entity>();

		std::vector<std::string>::const_iterator itr = std::find(data.begin(), data.end(), name);
		if (itr == data.end()) {
			BOOST_ASSERT(false);
			return -1;
		}

		return std::distance(data.begin(), itr);
	}

}}
