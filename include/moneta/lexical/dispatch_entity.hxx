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
#include "../limits/member.hxx"
#include "../traits/member_name.hxx"
#include "../traits/entity_name.hxx"
#include "../algorithm/dispatch_entity.hxx"

namespace moneta { namespace lexical {

	namespace detail {
		struct entity_name_is {
			const std::string _name;
	
			entity_name_is(const std::string& name)
			 : _name(name) {}

			template <class Entity>
			bool operator()(Entity*) const {
				return _name == moneta::traits::entity_name<Entity>::get();
			}
		};
	}

	template <class Entities, class Visitor>
	const Visitor& dispatch_entity(const std::string& name, const Visitor& visitor) {
		return algorithm::dispatch_entity<Entities>(visitor, detail::entity_name_is(name));
	}

}}
