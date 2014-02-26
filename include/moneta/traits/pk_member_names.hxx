#pragma once
#include "members.hxx"
#include "tuple.hxx" // for detail::get_result_type // TODO: Make this better.
#include "member_names.hxx"
#include "pk.hxx"
#include <vector>
#include <string>

namespace moneta { namespace traits {

	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(
		member_name,
		get_pk_member_names, 
		pk_members<EntityType>::type
	)

}}
