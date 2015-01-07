#pragma once
#include "../limits/member.hxx"
#include "../traits/member_names.hxx"
#include "../_aux/mplx_at_c.hxx"

namespace moneta { namespace lexical {

	template <class Entity, class Operation>
	void dispatch_member(const std::string& name, const Operation& operation) {
		const int ordinal = traits::get_member_name_index<Entity>(name);
		mplx::at_c<typename moneta::traits::members<Entity>::type>(ordinal, operation);
	}

}}
