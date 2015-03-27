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
#include "../../sql/generators/delete.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class Entity, class PkType>
	const size_t soci_delete(::soci::session& session, const PkType primary_key) {
		const typename traits::pk<Entity>::type pk(primary_key);
		::soci::statement statement = (
			session.prepare << sql::generators::delete_from_table_where_pk<Entity>(),
					   ::soci::use(pk)
		);

		statement.execute(true);
		return size_t(statement.get_affected_rows());
	}

}}}
