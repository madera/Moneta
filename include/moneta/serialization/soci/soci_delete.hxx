#pragma once

namespace moneta { namespace serialization { namespace soci {

template <class EntityType, class PkType>
const size_t soci_delete(::soci::session& session, const PkType primary_key) {
	const typename traits::pk<EntityType>::type pk(primary_key);
	::soci::statement statement = (
		session.prepare << sql::query::delete_from_table_where_pk<EntityType>(),
				   ::soci::use(pk)
	);

	statement.execute(true);
	return size_t(statement.get_affected_rows());
}

}}}
