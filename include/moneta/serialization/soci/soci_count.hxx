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
#include "detail/soci_includes.hxx"
#include "../../sql/traits/table_name.hxx"
#include <boost/format.hpp>

namespace moneta { namespace serialization { namespace soci {

	template <class Entity>
	const size_t soci_count(::soci::session& session) {
		size_t count;
		const std::string table = sql::traits::get_table_name<Entity>();
		session << boost::format("select count(*) from %s") % table, ::soci::into(count);
		return count;
	}

}}}
