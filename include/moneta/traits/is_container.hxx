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

//
// TODO: Will this stand the test of time?
//
// Currently we borrow the code from Boost Spirit. In the future, we may need to reimplement it.
//
#include <boost/spirit/home/support/container.hpp>

namespace moneta { namespace traits {
	
	template <typename T>
	struct is_container : boost::spirit::traits::is_container<T> {};

	//
	// For our purposes, std::string is NOT a container.
	// This differs with the Boost Spirit definition.
	//
	template <>
	struct is_container<std::string> : boost::false_type {};

	//

	template <typename T>
	struct isnt_container : boost::mpl::not_<
		is_container<T>
	> {};
}}
