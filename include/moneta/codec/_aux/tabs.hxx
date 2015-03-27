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

namespace moneta { namespace codec { namespace aux {

	#define MONETA_CODEC_TABS_MAX 32

	namespace {
		const char* tabs_cstr = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	}

	template <signed int N>
	struct tabs {
		enum  { size = MONETA_CODEC_TABS_MAX };
		static const char* get() {
			const size_t count = (N > 0)? N : 0;
			return &tabs_cstr[size - (count % size)];
		}
	};

}}}
