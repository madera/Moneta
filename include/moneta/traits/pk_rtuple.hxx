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
#include "../traits/pk.hxx"
#include "../traits/tuple.hxx"

namespace moneta { namespace traits {

	template <class Entity>
	struct pk_rtuple : moneta::traits::detail::deref_if_unary<
		typename boost::fusion::result_of::as_vector<
			typename boost::mpl::transform<
				typename moneta::traits::detail::mpl::pk<Entity>::type,
				moneta::traits::pk<boost::mpl::_1>
			>::type
		>::type
	> {};

}}
