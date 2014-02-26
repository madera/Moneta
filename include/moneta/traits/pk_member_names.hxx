#pragma once
#include "members.hxx"
#include "tuple.hxx" // for detail::get_result_type // TODO: Make this better.
#include "member_names.hxx"
#include <vector>
#include <string>

namespace moneta { namespace traits {

	template <class EntityType>
	std::vector<std::string> get_pk_member_names() {
		return detail::get_memptr_names<
			typename boost::mpl::apply<
				detail::get_pk_memptr_types,
				EntityType
			>::type
		>();
	}

}}
