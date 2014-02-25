#pragma once
#include "extract_pk.hxx"

namespace moneta { namespace traits {

	// FIXME: Do we really need this? Isn't vector's == enough?
	template <class EntityType>
	typename const bool same_pk(const EntityType& a, const EntityType& b) {
		return extract_pk(a) == extract_pk(b);
	}

}}
