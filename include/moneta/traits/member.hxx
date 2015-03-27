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
#include <boost/multi_index/member.hpp> // We reuse boost::multi_index::member<>

namespace moneta { namespace traits {

	template <class K, typename R, R K::* memptr>
	struct member : boost::multi_index::member<K, R, memptr> {
		typedef K class_type;

		static R K::* get() {
			return memptr;
		}
	};

	struct not_a_member {};
}}

#define MONETA_MEMBER(k, r, n) moneta::traits::member<k, r, &k::n>
