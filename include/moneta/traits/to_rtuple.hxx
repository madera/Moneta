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
#include "rtuple.hxx"
#include "../traits/detail/sepacon_opfx.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/tuple.hxx"
#include "../traits/pk.hxx"
#include "../traits/extract_pk.hxx"
#include "../traits/detail/blanker.hxx"
#include <boost/utility/enable_if.hpp>
#include <boost/fusion/algorithm/transformation.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class Entity>
		struct rtuple_maker {
			typename rtuple<Entity>::type
			operator()(Entity& entity) {
				return moneta::traits::detail::sepacon_opfx<
					typename moneta::traits::members<Entity>::type,
					typename rtuple<Entity>::type,
					Entity&,
					moneta::traits::get_pk_functor
				>()(entity);
			}
		};

	}

	template <class Entity>
	typename rtuple<Entity>::type
	to_rtuple(Entity& x) {
		return detail::rtuple_maker<Entity>()(x);
	}

	// XXX: Move this somewhere else.
	template <class Entity>
	typename traits::rtuple<Entity>::type make_rtuple() {
		typename traits::rtuple<Entity>::type result;
		return boost::fusion::transform(result, moneta::traits::detail::blanker());
	}

}}
