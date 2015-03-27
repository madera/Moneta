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
#include "../../algorithm/detail/path.hxx"
#include "tabs.hxx"

namespace moneta { namespace codec { namespace aux {

	namespace detail {

		template <class Path, signed int Extra = 0>
		struct tabs_impl {
			static const char* get() {
				using boost::mpl::size;
				using moneta::algorithm::detail::container_member_count;

				const signed n = 
					static_cast<signed>(size<Path>::value) +
					static_cast<signed>(container_member_count<Path>::value) + 
					static_cast<signed>(Extra)
				;
				
				const signed int count = (n >= 0)? n : 0;
				return tabs<count % MONETA_CODEC_TABS_MAX>::get();
			}
		};

	}

	template <class Path>
	inline const char* path_tabs() {
		return detail::tabs_impl<Path>::get();
	}

	template <class Path, signed int Extra>
	inline const char* path_tabs() {
		return detail::tabs_impl<Path, Extra>::get();
	}

}}}
