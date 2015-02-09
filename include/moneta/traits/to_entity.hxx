#pragma once
#include "to_tie.hxx"

namespace moneta { namespace traits {

	template <class Entity, class Tuple>
	Entity to_entity(const Tuple& tuple) {
		Entity entity;
		to_tie(entity) = tuple;
		return entity;
	}

}}
