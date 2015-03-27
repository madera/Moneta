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
#include "members.hxx"
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mpl/not.hpp>

namespace moneta { namespace traits {

	template <class Entity>
	struct is_entity : boost::mpl::not_<
		boost::is_same<typename members<Entity>::type, boost::false_type>
	> {};

	template <class Entity>
	struct isnt_entity : boost::mpl::not_<
		is_entity<Entity>
	> {};

}}
