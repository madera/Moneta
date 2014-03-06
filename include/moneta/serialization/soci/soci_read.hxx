#pragma once
#include "detail/soci_xlate_read.hxx"
#include "../../sql/generators/select.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class EntityType, class PkType>
	EntityType soci_read(::soci::session& session, const PkType primary_key) {
		sql::traits::db_tuple<EntityType>::type db_tuple;
		session << sql::generators::select_all_from_table_where_pk<EntityType>(),
			   ::soci::use(typename traits::pk<EntityType>::type(primary_key)),
			   ::soci::into(db_tuple);

		return detail::soci_xlate_read<EntityType>(session, db_tuple);
	}

}}}
