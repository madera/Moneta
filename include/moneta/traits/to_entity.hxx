#pragma once
#include "to_tie.hxx"

namespace moneta { namespace traits {

	template <class Entity, class TupleType>
	Entity to_entity(const TupleType& tuple) {
		Entity entity;
		to_tie(entity) = tuple;
		return entity;
	}

}}
