// XXX: Move this file into shell codec.

#pragma once
#include <boost/format.hpp>
#include <boost/fusion/include/at_c.hpp>
#include "../../traits/member_names.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"
#include "../../lexical/set_value.hxx"
#include <vector>
#include <map>
#include <string>
#include <boost/algorithm/string/trim.hpp>

namespace moneta { namespace serialization { namespace shell {

	namespace detail {

		//
		// Decoder
		//

		inline std::vector<std::string> special_split(std::string line) {
			boost::trim(line);
			if (line.size() >= 2) {
				if (line.front() == '{' && line.back() == '}') {
					line.erase(0, 1);
					line.erase(line.size() - 1);
				}
			}

			std::vector<std::string> result;

			std::string token;
			bool inside_sq_text = false;
			bool inside_dq_text = false;
			
			int level = 0;

			for (const char c : line) {
				if (c == ' ') {
					if (inside_sq_text || inside_dq_text || level != 0) {
						token += c;
					} else {
						if (!token.empty()) {
							result.push_back(token);
							token.clear();
						}
					}
				} else if (c == '\'') {
					if (inside_dq_text || level != 0) {
						token += c;
					} else {
						inside_sq_text = !inside_sq_text;
					}
				} else if (c == '"') {
					if (inside_sq_text || level != 0) {
						token += c;
					} else {
						inside_dq_text = !inside_dq_text;
					}
				} else if (c == '{') {
					++level;
					token += c;
					if (!inside_sq_text && !inside_dq_text && level != 0) {
						
					}
				} else if (c == '}') {
					--level;
					token += c;
					if (level == 0) {
						if (!token.empty()) {
							result.push_back(token);
							token.clear();
						}
					}
				} else {
					token += c;
				}
			}

			if (!token.empty()) {
				result.push_back(token);
			}

			return result;
		}

		inline std::map<std::string, std::string> line_to_kv(const std::string& line) {
			std::map<std::string, std::string> result;

			std::vector<std::string> split = special_split(line);

			std::vector<std::string>::const_iterator itr;
			for (itr = split.begin(); itr != split.end(); ++itr) {
				const std::string& s = *itr;

				const size_t pos = s.find('=');
				if (pos == std::string::npos) {
					result[s] = "";
				} else {
					const std::string key(s.begin(), s.begin() + pos);
					const std::string value(s.begin() + pos + 1, s.end());
					result[key] = value;
				}
			}

			return result;
		}

		template <class Entity>
		Entity from_kv(std::map<std::string, std::string>& kv, Entity& result = make_entity<Entity>()) {
			for (const auto& pair : kv) {
				const std::string& key = pair.first;
				const std::string& value = pair.second;

				const size_t index = moneta::traits::get_member_name_index<Entity>(key.c_str());
				lexical::set_value(result, index, value);
			}

			return result;
		}

	} // namespace detail

	template <class Entity>
	Entity from_line(const std::string& line, Entity& result = make_entity<Entity>()) {
		return detail::from_kv<Entity>(detail::line_to_kv(line), result);
	}

}}}
