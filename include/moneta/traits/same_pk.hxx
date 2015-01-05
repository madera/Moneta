#pragma once
#include "pk.hxx"

namespace moneta { namespace traits {

	// FIXME: Do we really need this? Isn't vector's == enough?
	template <class Entity>
	const bool same_pk(const Entity& a, const Entity& b) {
		return extract_pk(a) == extract_pk(b);
	}

}}
