#pragma once
#include "member_name.hxx"
#include "pk.hxx"

namespace moneta { namespace traits {

	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(
		member_name,
		std::string,
		get_pk_member_names, 
		typename pk_members<Entity>::type
	)

}}
