#pragma once
#include "extract_pk.hxx"

namespace moneta { namespace traits {

	// FIXME: Do we really need this? Isn't vector's == enough?
	template <class Entity>
	typename const bool same_pk(const Entity& a, const Entity& b) {
		return extract_pk(a) == extract_pk(b);
	}

}}
