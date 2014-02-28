#pragma once
#include <boost/format.hpp>
#include <boost/fusion/include/at_c.hpp>
#include "../detail/from_text_impl.hxx"
#include "../../traits/member_names.hxx"
#include "../../traits/detail/memptr_hacker.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"
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

		//
		// Encoder
		//

		template <class EntityType, class OstreamType = std::basic_ostream<char> >
		class member_ostreamer {
			const EntityType& _entity;
			OstreamType& _output;
		public:
			member_ostreamer(const EntityType& entity, OstreamType& output = std::cout)
				: _entity(entity), _output(output) {
			}

			template <typename T>
			void operator()(T& memptr) const {
				const size_t ordinal = traits::member_ordinal(memptr);
				const std::string k = traits::get_member_name<EntityType>(ordinal);
				const std::string v = textonize(memptr, _entity);
				
				const bool has_spaces = (v.find(' ') != std::string::npos);
				const bool has_brackets = !v.empty() && v[0] == '{';

				const char* format_string = (!has_brackets && has_spaces)? "%s='%s'" : "%s=%s";
				_output << boost::format(format_string) % k % v;

				if (ordinal + 1 != boost::mpl::size<traits::members<EntityType>::type>::value) {
					_output << ' ';
				}
			}
		};

		template <class EntityType, class Enable = void>
		struct textonator;

		template <class NonEntityType>
		struct textonator<
			NonEntityType,
			typename std::enable_if<
				!traits::is_entity<NonEntityType>::value
			>::type
		> {
			const std::string operator()(const NonEntityType& value) {
				// FIXME: This is awful... we should use our own serializer.
				// TODO: Rethink IO strategy.
				return boost::lexical_cast<std::string>(value);
			}
		};

		template<class EntityType>
		struct textonator<
			EntityType,
			typename std::enable_if<
				traits::is_entity<EntityType>::value
			>::type
		> {
			const std::string operator()(const EntityType& entity) {
				std::ostringstream oss;
				oss << '{';

				boost::fusion::for_each(
					traits::member_pointers<EntityType>::get(),
					member_ostreamer<EntityType, std::ostringstream>(entity, oss)
				);

				oss << '}';
				return oss.str();
			}
		};

		template <typename T, class K>
		const std::string textonize(T K::* memptr, const K& x) {
			return textonator<T>()(x.*memptr);
		}

	} // namespace detail

	template <class EntityType>
	EntityType from_kv(std::map<std::string, std::string>& kv, EntityType& result = make_entity<EntityType>()) {
		serialization::detail::from_text_impl<EntityType> text_assigner;
		for (const auto& pair : kv) {
			const std::string& key = pair.first;
			const std::string& value = pair.second;

			const size_t index = moneta::traits::get_member_name_index<EntityType>(key.c_str());
			text_assigner(result, index, value);
		}

		return result;
	}

	template <class EntityType>
	EntityType from_line(const std::string& line, EntityType& result = make_entity<EntityType>()) {
		return from_kv<EntityType>(detail::line_to_kv(line), result);
	}

	template <typename EntityType>
	const std::string to_line(const EntityType& entity) {
		return detail::textonator<EntityType>()(entity);
	}

}}}
