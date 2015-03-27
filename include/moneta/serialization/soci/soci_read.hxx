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
#include "detail/soci_xlate_read.hxx"
#include "../../sql/generators/select.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class Entity, class PkType>
	Entity soci_read(::soci::session& session, const PkType primary_key) {
		traits::rtuple<Entity>::type rtuple;
		session << sql::generators::select_all_from_table_where_pk<Entity>(),
			   ::soci::use(typename traits::pk<Entity>::type(primary_key)),
			   ::soci::into(rtuple);

		return detail::soci_xlate_read<Entity>(session, rtuple);
	}

}}}
