#pragma once
#include <boost/fusion/include/at_c.hpp>
#include "../detail/from_text_impl.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"
#include <vector>
#include <map>
#include <string>

namespace moneta { namespace serialization { namespace shell {

	namespace detail {

		std::vector<std::string> special_split(const std::string& line) {
			std::vector<std::string> result;

			std::string token;
			bool inside_sq_text = false;
			bool inside_dq_text = false;

			for (const char c : line) {
				if (c == ' ') {
					if (inside_sq_text || inside_dq_text) {
						token += c;
					} else {
						if (!token.empty()) {
							result.push_back(token);
							token.clear();
						}
					}
				} else if (c == '\'') {
					if (inside_dq_text) {
						token += c;
					} else {
						inside_sq_text = !inside_sq_text;
					}
				} else if (c == '"') {
					if (inside_sq_text) {
						token += c;
					} else {
						inside_dq_text = !inside_dq_text;
					}
				} else {
					token += c;
				}
			}

			if (!token.empty())
				result.push_back(token);

			return result;
		}

		std::map<std::string, std::string> line_to_kv(const std::string& line) {
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
			EntityType& _entity;
			OstreamType& _output;
		public:
			member_ostreamer(EntityType& entity, OstreamType& output = std::cout)
				: _entity(entity), _output(output) {
			}

			template <typename T>
			void operator()(T& memptr) const {
				const size_t ordinal = fucker::member_ordinal(memptr);
				const std::string name = fucker::name<EntityType>(ordinal);
				_output << boost::format("%s=%s ") % name % 
					textonator<
						std::remove_reference<decltype(_entity.*memptr)>::type // XXX: C++03!
					>()(_entity.*memptr);
			}

			OstreamType& output() {
				return _output;
			}
		};



		template <class EntityType, class Enable = void>
		struct textonator;

		template <class NonEntityType>
		struct textonator<NonEntityType, typename std::enable_if<
			!moneta::traits::is_entity<
				typename std::remove_reference<NonEntityType>::type
			>::value
		>::type> {
			const std::string operator()(const NonEntityType& value) {
				return boost::lexical_cast<std::string>(value);
			}
		};

		template<class EntityType>
		struct textonator<EntityType, typename std::enable_if<
			moneta::traits::is_entity<
				typename std::remove_reference<EntityType>::type
			>::value
		>::type> {
			const std::string operator()(const EntityType& entity) {
				std::ostringstream oss;
				member_ostreamer<EntityType, std::ostringstream> functor(const_cast<EntityType&>(entity), oss);
				boost::fusion::for_each(traits::member_pointers<EntityType>::get(), functor);
				return functor.output().str();
			}
		};

	} // namespace detail

	template <class EntityType>
	EntityType from_kv(std::map<std::string, std::string>& kv, EntityType& result = make_entity<EntityType>()) {
		fucker::from_text_impl<EntityType> text_assigner;
		for (const auto& pair : kv) {
			const std::string& key = pair.first;
			const std::string& value = pair.second;

			const size_t index = fucker::name_index<EntityType>(key.c_str());
			text_assigner(result, index, value);
		}

		return result;
	}

	template <class EntityType>
	EntityType from_line(const std::string& line, EntityType& result = make_entity<EntityType>()) {
		std::cerr << "from_line: " << line << std::endl;
		return from_kv<EntityType>(detail::line_to_kv(line), result);
	}

	template <typename EntityType>
	const std::string to_line(const EntityType& entity) {
		return detail::textonator<EntityType>()(entity);
	}

}}}
