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
#include <boost/fusion/include/as_vector.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/transform.hpp>

// For our users...
// FIXME: Get this somewhere... but not here.
#include <boost/fusion/include/at_c.hpp>
#include <boost/mpl/at.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class Entity>
		struct get_result_type {
			template <typename T>
			struct apply {
				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename boost::add_const<
						typename T::result_type
					>::type,
					typename T::result_type
				>::type type;
			};
		};

		namespace mpl {

			template <class Entity>
			struct vector : boost::mpl::transform<
				typename members<Entity>::type,
				get_result_type<Entity>
			> {};

		}
	}

	template <class Entity>
	struct tuple : boost::fusion::result_of::as_vector<
		typename detail::mpl::vector<Entity>::type
	> {};

}}
