#pragma once
#include "detail/generate_params.hxx"
#include "../../traits/pk_member_names.hxx"
#include "../traits/field_names.hxx"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace moneta { namespace sql { namespace generators {

	template <class Entity>
	const std::string select_all_from_table() {
		std::ostringstream oss;
		oss << "SELECT " << boost::join(traits::get_field_names<Entity>(), ", ") << '\n'
		    << "  FROM " << traits::get_table_name<Entity>();
		return oss.str();
	}

	template <class Entity>
	const std::string select_all_from_table_where_pk() {
		std::ostringstream oss;
		oss << select_all_from_table<Entity>()
		    << "\n WHERE " << detail::generate_filter_parameters<Entity>();
		return oss.str();
	}

}}}
