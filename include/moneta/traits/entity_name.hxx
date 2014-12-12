#pragma once
#include "detail/member_trait_base.hxx"

MONETA_DECLARE_ENTITY_TRAIT(entity_name)

#define MONETA_ENTITY_NAME(klass, field) \
	MONETA_DEFINE_TRAIT_WITH_GET(entity_name, (klass), std::string, BOOST_PP_STRINGIZE(field))

namespace moneta { namespace traits {
	MONETA_DEFINE_ENTITY_TRAIT_GETTER(entity_name, get_entity_name)
}}
