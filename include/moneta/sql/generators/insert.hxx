#pragma once
#include "detail/generate_params.hxx"
#include "../../traits/pk_member_names.hxx"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace moneta { namespace sql { namespace generators {

	template <class EntityType>
	const std::string insert_into_table() {
		const std::vector<std::string> fields = traits::get_field_names<EntityType>();

		std::ostringstream oss;
		oss << "INSERT INTO " << traits::get_table_name<EntityType>() << " (\n"
		    << '\t' << boost::join(fields, ", ") << '\n'
		    << ") VALUES (\n"
		    << '\t' << boost::algorithm::join(detail::generate_params(fields.size()), ", ") << '\n'
		    << ")";
		return oss.str();
	}

}}}
