#pragma once
#include <vector>
#include <string>

namespace moneta { namespace sql { namespace generators { namespace detail {

	inline std::vector<std::string> generate_params(const size_t quantity) {
		std::vector<std::string> result;
		for (size_t i=0; i<quantity; ++i) {
			std::string p = ":";
			p += char('a' + i);
			result.push_back(p);
		}

		return result;
	}

}}}}
