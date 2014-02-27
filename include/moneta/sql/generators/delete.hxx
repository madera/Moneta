#pragma once
#include "detail/generate_params.hxx"
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
		    << "\n WHERE " << detail::generate_parameters_k_eq_v<EntityType>();
		return oss.str();
	}

}}}
