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
#include <boost/mpl/fold.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/contains.hpp>

namespace mplx {

	template <class Sequence>
	struct remove_duplicates {
		typedef typename boost::mpl::fold< 
			Sequence,
			boost::mpl::pair<
				typename boost::mpl::clear<Sequence>::type,
				boost::mpl::set0<>
			>,
			boost::mpl::if_<
				boost::mpl::contains<
					boost::mpl::second<boost::mpl::_1>,
					boost::mpl::_2
				>,
				boost::mpl::_1,
				boost::mpl::pair<
					boost::mpl::push_back<
						boost::mpl::first<boost::mpl::_1>,
						boost::mpl::_2
					>,
					boost::mpl::insert<
						boost::mpl::second<boost::mpl::_1>,
						boost::mpl::_2
					>
				>
			>
		>::type::first type;
	};

}
