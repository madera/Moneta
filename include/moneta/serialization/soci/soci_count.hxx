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
