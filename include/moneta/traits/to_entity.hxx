#pragma once
#include "to_tie.hxx"

namespace moneta { namespace traits {

	template <class EntityType, class TupleType>
	EntityType to_entity(const TupleType& tuple) {
		EntityType entity;
		to_tie(entity) = tuple;
		return entity;
	}

}}
