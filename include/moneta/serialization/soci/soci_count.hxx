#pragma once
#include "detail/soci_includes.hxx"
#include "../../sql/traits/table_name.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class EntityType>
	const size_t soci_count(::soci::session& session) {
		const std::string table = traits::get_table_name<EntityType>();

		int count; // XXX: Must be size_t
		session << boost::format("select count(*) from %s") % table, ::soci::into(count);

		return count;
	}

}}}
