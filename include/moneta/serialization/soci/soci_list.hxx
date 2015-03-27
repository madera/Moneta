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
#include "../../sql/generators/select.hxx"
#include "detail/soci_xlate_read.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class Entity, class OutputIteratorType>
	const size_t soci_list(::soci::session& session, OutputIteratorType& output) {
		typedef traits::rtuple<Entity>::type rtuple_type;
		::soci::rowset<rtuple_type> rows = (
			session.prepare << sql::generators::select_all_from_table<Entity>()
		);
	
		size_t count = 0;
		for (::soci::rowset<rtuple_type>::const_iterator itr = rows.begin();
			itr != rows.end(); ++itr) {
			Entity newcomer = detail::soci_xlate_read<Entity>(session, *itr);
			*output++ = newcomer;
			++count;
		}

		return count;
	}

}}}
