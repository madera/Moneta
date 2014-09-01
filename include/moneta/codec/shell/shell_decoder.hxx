#pragma once
#include "../decoder.hxx"
#include "../../serialization/shell/shell_codec.hxx"

namespace moneta { namespace codec {

	struct shell;

	template <class Entity>
	struct entity_decoder<shell, Entity> {
		
		template <class Entity, class Iterator>
		int operator()(Entity& entity, Iterator begin, Iterator end) const {
			entity = moneta::serialization::shell::from_line<Person>(std::string(begin, end));
			return 1;
		}

	};

}}
