#pragma once
#include "detail/generate_params.hxx"
#include "../../traits/pk_member_names.hxx"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace moneta { namespace sql { namespace generators {

	template <class EntityType>
	const std::string select_all_from_table() {
		std::ostringstream oss;
		oss << "SELECT " << boost::join(traits::get_field_names<EntityType>(), ", ") << '\n'
		    << "  FROM " << traits::get_table_name<EntityType>();
		return oss.str();
	}

	template <class EntityType>
	const std::string select_all_from_table_where_pk() {
		std::ostringstream oss;
		oss << select_all_from_table<EntityType>()
		    << "\n WHERE " << oss.str();
		
		const std::vector<std::string> fields = moneta::traits::get_pk_member_names<EntityType>();
		
		for (size_t i=0; i < fields.size(); ++i) {
			oss << boost::format("%s = :%s") % fields[i] % boost::to_lower_copy(fields[i]);
			if (i+1 != fields.size()) {
				oss << " AND ";
			}
		}

		return oss.str();
	}

}}}
