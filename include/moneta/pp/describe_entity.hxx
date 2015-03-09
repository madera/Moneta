#pragma once
#include "detail/moneta_pp.hxx"
#include "../traits/entity_name.hxx"
#include "../traits/member.hxx"
#include "../traits/member_name.hxx"
#include <boost/mpl/vector.hpp>

// TODO: Rename file and change its location.

#define MONETA_DESCRIBE_ENTITY(entity, members)               \
	MONETA_ENTITY_NAME(entity, entity)                    \
	MONETA_PP_EXPAND_ENTITY_MEMBERS(entity, members)      \
	MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(entity, members)

#define MONETA_DESCRIBE_ENTITY_NS(ns_seq, entity, members) \
	MONETA_DEFINE_TRAIT_WITH_GET( \
		entity_name, \
		MONETA_PP_NAMESPACE_JOIN(ns_seq, ::) entity, \
		std::string, \
		BOOST_PP_STRINGIZE(entity) \
	) \
	MONETA_PP_EXPAND_ENTITY_MEMBERS(MONETA_PP_NAMESPACE_JOIN(ns_seq, ::) entity, members)      \
	MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(MONETA_PP_NAMESPACE_JOIN(ns_seq, ::) entity, members)

//

#define MONETA_DEFINE_ENTITY(entity, members)    \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members)       \
	MONETA_ENTITY_NAME(entity, entity)                    \
	MONETA_PP_EXPAND_ENTITY_MEMBERS(entity, members)      \
	MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(entity, members)

#define MONETA_DEFINE_ENTITY_NS(ns_seq, entity, members)   \
	MONETA_PP_NAMESPACE_BEGIN(ns_seq)                  \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members)    \
	MONETA_PP_NAMESPACE_END(ns_seq)                    \
	MONETA_DESCRIBE_ENTITY_NS(ns_seq, entity, members)
