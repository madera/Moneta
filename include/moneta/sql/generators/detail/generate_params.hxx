#pragma once
#include <vector>
#include <string>

// XXX: Organize this...

namespace moneta { namespace sql { namespace generators { namespace detail {

	inline std::vector<std::string> generate_parameters(const size_t quantity) {
		std::vector<std::string> result;
		for (size_t i=0; i<quantity; ++i) {
			std::string p = ":";
			p += char('a' + i);
			result.push_back(p);
		}

		return result;
	}
	
	template <class EntityType>
	const std::string generate_parameters_k_eq_v() {
		const std::vector<std::string> fields = moneta::sql::traits::get_pk_field_names<EntityType>();
		
		std::ostringstream oss;
		for (size_t i=0; i < fields.size(); ++i) {
			oss << boost::format("%s = :%s") % fields[i] % boost::to_lower_copy(fields[i]);
			if (i+1 != fields.size()) {
				oss << " AND ";
			}
		}

		return oss.str();
	}

	template <class EntityType>
	const std::string generate_parameters(const std::string& separator = ", ") {
		const std::vector<std::string> fields = moneta::sql::traits::get_field_names<EntityType>();
		
		std::ostringstream oss;
		for (size_t i=0; i < fields.size(); ++i) {
			oss << ':' << boost::to_lower_copy(fields[i]);
			if (i+1 != fields.size()) {
				oss << separator;
			}
		}

		return oss.str();
	}

}}}}
