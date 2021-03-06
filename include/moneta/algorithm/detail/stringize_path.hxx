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

// TODO: Move to proper path.

#pragma once
#include "../../traits/entity_name.hxx"
#include "../../traits/member_name.hxx"

namespace moneta { namespace codec { namespace detail {

	struct path_printer {
		std::ostringstream& oss;

		path_printer(std::ostringstream& oss_)
		 : oss(oss_) {}

		template <class Member>
		void operator()(Member&) const {
			oss << '/'
				<< moneta::traits::get_entity_name<typename Member::class_type>()
				<< "::"
				<< moneta::traits::member_name<Member>::get();
		}
	};

	template <class Path>
	typename boost::disable_if<boost::mpl::is_sequence<Path>, std::string>::type
	stringize_path() {
		return "<no_path>";
	}

	template <class Path>
	typename boost::enable_if<boost::mpl::is_sequence<Path>, std::string>::type
	stringize_path() {
		std::ostringstream oss;
		path_printer state(oss);
		boost::mpl::for_each<Path>(state);
		return state.oss.str();
	}

}}}
