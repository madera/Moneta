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
