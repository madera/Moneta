#pragma once
#include "../decoder.hxx"
#include "../../lexical/dispatch_entity.hxx"
#include "../../lexical/dispatch_member.hxx"
#include "../../make_entity.hxx"
#include "../group_decoder.hxx"
#include "../_aux/io/copy_while.hxx"
#include "../_aux/io/consume_while.hxx"
#include "../_aux/io/consume_whitespaces.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <map>

namespace moneta { namespace codec { namespace shell_decoder_implementation {

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

	template <class Entity>
	struct assign_or_recurse;

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
	Entity from_kv(const std::map<std::string, std::string>& kv) {
		Entity result = make_entity<Entity>();
		for (const auto& pair : kv) {
			const std::string& key = pair.first;
			const std::string& value = pair.second;

			moneta::lexical::dispatch_member<Entity>(key, assign_or_recurse<Entity>(result, value));
		}

		return result;
	}

	template <class Entity>
	Entity from_line(const std::string& line) {
		return from_kv<Entity>(line_to_kv(line));
	}

	template <class Entity>
	struct assign_or_recurse {
		Entity& _entity;
		const std::string& _value;

		assign_or_recurse(Entity& entity, const std::string& value)
			: _entity(entity), _value(value) {}

		template <class Member>
		typename boost::enable_if<
			moneta::traits::is_entity<
				typename Member::result_type
			>
		>::type
		operator()() const {
			Member()(_entity) = from_line<typename Member::result_type>(_value);
		}

		template <class Member>
		typename boost::disable_if<
			moneta::traits::is_entity<
				typename Member::result_type
			>
		>::type
		operator()() const {
			Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
		}
	};

	struct shell_prefix_reader {
		typedef std::string type;
	
		struct is_identifier_char {
			typedef char argument_type;

			bool operator()(const char c) const {
				return std::isalnum(c) || c == '_';
			}
		};

		template <class Iterator>
		int operator()(Iterator begin, Iterator end, type& prefix) const {
			const size_t minimum = 2; // The bare minimum: "a "
			const size_t available = std::distance(begin, end);
			if (available < minimum) {
				return 0 - (minimum - available);
			}

			Iterator itr = begin;
			const int whitespaces = moneta::codec::io::consume_whitespaces(itr, end);
			itr += whitespaces;

			int result = moneta::codec::io::copy_while(
				itr, end, std::back_inserter(prefix), is_identifier_char()
			);
			itr += result;

			// If we've reached the end, we can't prove it's the complete entity name.
			if (itr == end) {
				return -1;
			}

			return std::distance(begin, itr);
		}
	};

	struct shell_decoder_start_entity {
		template <class Iterator, class Entity, class Path>
		int operator()(Iterator begin, Iterator end, Entity& entity, Path) const {
			std::string prefix;
			const int result = shell_prefix_reader()(begin, end, prefix);
			if (result <= 0) {
				return result;
			}

			Iterator itr = begin + result;
			if (itr != end && *itr == '=') {
				++itr;
			}

			entity = from_line<Entity>(std::string(itr, end));
			//return std::distance(begin, itr);
			return std::distance(begin, end); // XXX
		}
	};

	template <class EntityGroup>
	struct shell_decoder {
		typedef moneta::codec::group_decoder<
			moneta::codec::decoder<
				moneta::codec::start_actions<shell_decoder_start_entity>
			>,
			EntityGroup,
			shell_prefix_reader
		> type;
	};

}}}

namespace moneta { namespace codec { namespace detail {

	template <class Entity>
	struct prefix_value<shell_decoder_implementation::shell_prefix_reader, Entity> {
		static std::string get() {
			return moneta::traits::get_entity_name<Entity>();
		}
	};

}}}

namespace moneta { namespace codec {
	using shell_decoder_implementation::shell_decoder;
}}
