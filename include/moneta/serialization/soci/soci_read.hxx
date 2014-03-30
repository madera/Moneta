#pragma once
#include "detail/soci_xlate_read.hxx"
#include "../../sql/generators/select.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class EntityType, class PkType>
	EntityType soci_read(::soci::session& session, const PkType primary_key) {
		sql::traits::rtuple<EntityType>::type rtuple;
		session << sql::generators::select_all_from_table_where_pk<EntityType>(),
			   ::soci::use(typename traits::pk<EntityType>::type(primary_key)),
			   ::soci::into(rtuple);

		return detail::soci_xlate_read<EntityType>(session, rtuple);
	}

}}}
