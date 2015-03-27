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
#include "../lexical/dispatch_member.hxx"
#include <boost/lexical_cast.hpp>
#include <string>

namespace moneta { namespace lexical {

	namespace detail {

		template <class Entity>
		struct lexical_setter {
			Entity& _entity;
			const std::string& _value;

			lexical_setter(Entity& entity, const std::string& value)
			 : _entity(entity), _value(value) {}

			template <class Member>
			typename boost::enable_if<moneta::traits::is_entity<typename Member::result_type> >::type
			operator()() const {
			}

			template <class Member>
			typename boost::disable_if<moneta::traits::is_entity<typename Member::result_type> >::type
			operator()() const {
				Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
			}
		};

	}

	template <class Entity>
	void set_value(Entity& entity, const std::string member, const std::string& value) {
		moneta::lexical::dispatch_member<Entity>(member, detail::lexical_setter<Entity>(entity, value));
	}

}}
