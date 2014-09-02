#pragma once
#include "../lexical/dispatch_member.hxx"
#include <boost/lexical_cast.hpp>
#include <string>

namespace moneta { namespace lexical {

	namespace detail {

		template <class Entity>
		struct lexical_setter {
			Entity& _entity;
			const std::string& _value;

			lexical_setter(Entity& entity, const std::string& value)
			 : _entity(entity), _value(value) {}

			template <class Member>
			typename boost::enable_if<moneta::traits::is_entity<typename Member::result_type> >::type
			operator()() const {
			}

			template <class Member>
			typename boost::disable_if<moneta::traits::is_entity<typename Member::result_type> >::type
			operator()() const {
				Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
			}
		};

	}

	template <class Entity>
	void set_value(Entity& entity, const std::string name, const std::string& value) {
		moneta::lexical::dispatch_member<Entity>(name, detail::lexical_setter<Entity>(entity, value));
	}

	template <class Entity>
	void set_value(Entity& entity, const int ordinal, const std::string& value) {
		moneta::lexical::dispatch_member<Entity>(ordinal, detail::lexical_setter<Entity>(entity, value));
	}

}}
