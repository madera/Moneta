#pragma once
#include "../limits/member.hxx"
#include "../traits/member_names.hxx"
#include "../algorithm/dispatch_entity.hxx"

namespace moneta { namespace lexical {

	namespace detail {
		struct entity_name_is {
			std::string _name;
	
			entity_name_is(const std::string& name)
			 : _name(name) {}

			template <class Entity>
			bool operator()() const {
				return _name == moneta::traits::detail::entity_name<Entity>::get();
			}
		};
	}

	template <class Entities, class Visitor>
	void dispatch_entity(const std::string& name, Visitor& visitor) {
		algorithm::dispatch_entity<Entities>(visitor, detail::entity_name_is(name));
	}

}}
