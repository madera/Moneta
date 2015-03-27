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
#include <boost/mpl/clear.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>

namespace mplx {

	template<class Sequence> 
	struct flatten : boost::mpl::fold<
		Sequence,
		typename boost::mpl::clear<Sequence>::type,
		boost::mpl::if_<
			boost::mpl::is_sequence<boost::mpl::_2>,
			boost::mpl::copy<
				mplx::flatten<boost::mpl::_2>,
				boost::mpl::back_inserter<boost::mpl::_1>
			>,
			boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
		>
	> {};

}
