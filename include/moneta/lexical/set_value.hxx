#pragma once
#include "detail/from_text_impl.hxx"
#include <boost/lexical_cast.hpp>
#include <string>

namespace moneta { namespace lexical {

	template <class Entity>
	void set_value(Entity& entity, const std::string name, const std::string& value) {
		const int ordinal = moneta::traits::get_member_name_index<Entity>(name);
		moneta::serialization::detail::from_text_impl<Entity>()(entity, ordinal, value);
	}

	template <class Entity>
	void set_value(Entity& entity, const int ordinal, const std::string& value) {
		moneta::serialization::detail::from_text_impl<Entity>()(entity, ordinal, value);
	}

	template <class Member, class Entity>
	void set_value(Entity& entity, const std::string& value) {
		Member()(entity) = boost::lexical_cast<typename Member::result_type>(value);
	}

}}
