#pragma once
#include "../decoder.hxx"
#include "../../lexical/dispatch_member.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <map>

namespace moneta { namespace codec {
	
	namespace shell_detail {

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
		struct assign_or_recurse {
			Entity& _entity;
			const std::string& _value;

			assign_or_recurse(Entity& entity, const std::string& value)
				: _entity(entity), _value(value) {}

			template <class Member>
			typename boost::enable_if<moneta::traits::is_entity<typename Member::result_type> >::type
			operator()() const {
				from_line(_value, Member()(_entity));
			}

			template <class Member>
			typename boost::disable_if<moneta::traits::is_entity<typename Member::result_type> >::type
			operator()() const {
				Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
			}
		};

		template <class Entity>
		Entity from_kv(std::map<std::string, std::string>& kv, Entity& result = make_entity<Entity>()) {
			for (const auto& pair : kv) {
				const std::string& key = pair.first;
				const std::string& value = pair.second;

				moneta::lexical::dispatch_member<Entity>(key, assign_or_recurse<Entity>(result, value));
			}

			return result;
		}

		template <class Entity>
		Entity from_line(const std::string& line, Entity& result = make_entity<Entity>()) {
			return from_kv<Entity>(line_to_kv(line), result);
		}
	
	}

	struct shell;

	template <class Entity>
	struct entity_decoder<shell, Entity> {
		
		template <class Entity, class Iterator>
		int operator()(Entity& entity, Iterator begin, Iterator end) const {
			entity = shell_detail::from_line<Entity>(std::string(begin, end));
			return end - begin; // XXX: FIXME: This is bad.
		}

	};

}}
