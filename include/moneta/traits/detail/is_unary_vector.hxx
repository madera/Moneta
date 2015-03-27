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
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal_to.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class Sequence>
	struct is_unary_vector : boost::mpl::equal_to<
		typename boost::mpl::size<Sequence>::type,
		boost::mpl::int_<1>
	> {};

}}}
