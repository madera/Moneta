// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "../../traits/field_names.hxx"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

// XXX: Organize this...

namespace moneta { namespace sql { namespace generators { namespace detail {

	//inline std::vector<std::string> generate_parameters(const size_t quantity) {
	//	std::vector<std::string> result;
	//	for (size_t i=0; i<quantity; ++i) {
	//		std::string p = ":";
	//		p += char('a' + i);
	//		result.push_back(p);
	//	}
	//
	//	return result;
	//}
	
	template <class Entity>
	const std::string generate_filter_parameters() {
		const std::vector<std::string> fields = moneta::sql::traits::get_pk_field_names<Entity>();
		
		std::ostringstream oss;
		for (size_t i=0; i < fields.size(); ++i) {
			oss << boost::format("%s = :%s") % fields[i] % boost::to_lower_copy(fields[i]);
			if (i+1 != fields.size()) {
				oss << " AND ";
			}
		}

		return oss.str();
	}

	// TODO: Rename this to something appropriate.
	template <class Entity>
	const std::string generate_parameters(const std::string& separator = ", ") {
		const std::vector<std::string> fields = moneta::sql::traits::get_field_names<Entity>();
		
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
