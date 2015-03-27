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
#include "rtie.hxx"
#include "../traits/detail/sepacon_opfx.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/tie.hxx"
#include "../traits/pk.hxx"
#include "../traits/extract_pk.hxx"
#include <boost/utility/enable_if.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class Entity>
		struct rtie_maker {
			typename rtie<Entity>::type
			operator()(Entity& entity) {
				return moneta::traits::detail::sepacon_opfx<
					typename moneta::traits::members<Entity>::type,
					typename rtie<Entity>::type,
					Entity&,
					moneta::traits::get_pk_tie_functor
				>()(entity);
			}
		};

	}

	template <class Entity>
	typename rtie<Entity>::type
	to_rtie(Entity& x) {
		return detail::rtie_maker<Entity>()(x);
	}

}}
