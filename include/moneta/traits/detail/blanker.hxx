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
#include "pure_type.hxx"
#include "../is_entity.hxx"

//#include "../../make_entity.hxx"

// XXX: Make this make_entity_fwd.hxx
namespace moneta {
	template <class Entity>
	typename traits::pure_type<Entity>::type make_entity();
}

namespace moneta { namespace traits { namespace detail {

	template <typename T>
	typename boost::enable_if<traits::is_entity<T>, T>::type
	make_blank() {
		return moneta::make_entity<T>();
	}

	template <typename T>
	typename boost::disable_if<traits::is_entity<T>, T>::type
	make_blank() {
		return T();
	}

	struct blanker {
		template<typename Sig>
		struct result;

		template<typename T>
		struct result<blanker(T)> : traits::pure_type<T> {};

		template <typename T>
		typename result<blanker(T)>::type
		operator()(T&) const {
			return make_blank<
				typename result<blanker(T)>::type
			>();
		}
	};

}}}
