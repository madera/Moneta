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
#include <boost/type_traits/integral_constant.hpp>

namespace moneta { namespace codec { namespace detail {

	template <class PrefixReader, class Entity>
	struct prefix_value : boost::false_type {};

}}}

#define MONETA_CODEC_PREFIX_VALUE(prefix_reader, entity, value) \
	namespace moneta { namespace codec { namespace detail { \
		template <>                                     \
		struct prefix_value<prefix_reader, entity> {    \
			static prefix_reader::type get() {      \
				return value;                   \
			}                                       \
		};                                              \
	}}}
