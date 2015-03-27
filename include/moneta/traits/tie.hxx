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
#include "tuple.hxx"

namespace moneta { namespace traits {

	template <typename Entity>
	struct tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename detail::mpl::vector<Entity>::type,
			boost::add_reference<boost::mpl::_1>
		>::type
	> {};

}}
