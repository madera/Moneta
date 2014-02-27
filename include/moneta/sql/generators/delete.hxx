#pragma once
#include "../../traits/pk_member_names.hxx"
#include "../traits/field_names.hxx"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace moneta { namespace sql { namespace generators {

	template <class EntityType>
	const std::string delete_from_table() {
		std::ostringstream oss;
		oss << "DELETE FROM " << traits::get_table_name<EntityType>();
		return oss.str();
	}

	template <class EntityType>
	const std::string delete_from_table_where_pk() {
		std::ostringstream oss;
		oss << delete_from_table<EntityType>()
		    << "\n WHERE " << oss.str();

		const std::vector<std::string> fields = moneta::sql::traits::get_pk_field_names<EntityType>();
		
		for (size_t i=0; i < fields.size(); ++i) {
			oss << boost::format("%s = :%s") % fields[i] % boost::to_lower_copy(fields[i]);
			if (i+1 != fields.size()) {
				oss << " AND ";
			}
		}

		return oss.str();
	}

}}}
