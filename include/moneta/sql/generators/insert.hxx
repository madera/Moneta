#pragma once
#include "detail/generate_params.hxx"
#include "../../traits/pk_member_names.hxx"
#include "../traits/table_name.hxx"
#include "../traits/field_names.hxx"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace moneta { namespace sql { namespace generators {

	template <class Entity>
	const std::string insert_into_table() {
		const std::vector<std::string> fields = traits::get_field_names<Entity>();

		std::ostringstream oss;
		oss << "INSERT INTO " << traits::get_table_name<Entity>() << " (\n"
		    << '\t' << boost::join(fields, ", ") << '\n'
		    << ") VALUES (\n"
		    << '\t' << detail::generate_parameters<Entity>() << '\n'
		    << ")";
		return oss.str();
	}

}}}
