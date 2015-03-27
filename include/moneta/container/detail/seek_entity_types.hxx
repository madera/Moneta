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

// XXX: Move this file.
#pragma once
#include "../../traits/tuple.hxx"
#include "../../traits/is_entity.hxx"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/optional.hpp>

namespace moneta { namespace container { namespace detail {

	template <class Entity, class State = boost::mpl::vector<> >
	struct seek_entity_types : boost::mpl::fold<
		typename traits::tuple<Entity>::type,
		typename boost::mpl::push_back<State, Entity>::type,
		boost::mpl::if_<
			traits::is_entity<boost::mpl::_2>,
			moneta::container::detail::seek_entity_types<boost::mpl::_2, boost::mpl::_1>,
			boost::mpl::_1
		>
	> {};

}}}
