#pragma once
#include "detail/soci_xlate_read.hxx"

namespace moneta { namespace serialization { namespace soci {

template <class EntityType, class PkType>
EntityType soci_read(::soci::session& session, const PkType primary_key) {
	const typename traits::pk<EntityType>::type pk(primary_key);	
	fucker::traits::sql::db_tuple<EntityType>::type db_tuple;

	std::cout << "++ READ "  << traits::sql::table_name<traits::pure_type<EntityType>::type>::get()
		  << " PK: " << pk << std::endl;

	session << sql::query::select_all_from_table_where_pk<EntityType>(),
		   ::soci::use(pk), ::soci::into(db_tuple);

	return detail::soci_xlate_read<EntityType>(session, db_tuple);
}

}}}
