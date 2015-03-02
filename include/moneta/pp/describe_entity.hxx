#pragma once
#include "detail/moneta_pp.hxx"
#include "../traits/entity_name.hxx"
#include "../traits/member.hxx"
#include "../traits/member_name.hxx"
#include <boost/mpl/vector.hpp>

// ---

#define MONETA_PP_MEMBER_FROM_TNT_PAIR(entity, pair) \
	MONETA_MEMBER(entity, BOOST_PP_TUPLE_ELEM(2, 0, pair), BOOST_PP_TUPLE_ELEM(2, 1, pair))

// ---

#define __MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(r, entity, pair)                                       \
	namespace moneta { namespace traits {                                                         \
		template <>                                                                           \
		struct member_name<MONETA_PP_MEMBER_FROM_TNT_PAIR(entity, pair)> : boost::true_type { \
			typedef std::string trait_type;                                               \
			static trait_type get() {                                                     \
				return BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, pair));           \
			}                                                                             \
		};                                                                                    \
	}}

#define MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(entity, members) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES, entity, members)

// ---

#define __MONETA_PP_EXPAND_ENTITY_MEMBERS(r, entity, pair) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(r, 2)) MONETA_PP_MEMBER_FROM_TNT_PAIR(entity, pair)

#define MONETA_PP_EXPAND_ENTITY_MEMBERS(entity, members)                                          \
	namespace moneta { namespace traits {                                                     \
		template <>                                                                       \
		struct members_of<entity> : boost::mpl::vector<                                   \
			BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_ENTITY_MEMBERS, entity, members) \
		>{};                                                                              \
	}}

// ---

#define __MONETA_PP_DEFINE_MEMBERS(r, _, tuple) \
	BOOST_PP_TUPLE_ELEM(2, 0, tuple) BOOST_PP_TUPLE_ELEM(2, 1, tuple);

#define MONETA_PP_DEFINE_MEMBERS(members) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_DEFINE_MEMBERS, _, members)

// ---

#define MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members) \
	struct entity {                                 \
		MONETA_PP_DEFINE_MEMBERS(members)       \
	};

// ---

#define MONETA_DEFINE_AND_DESCRIBE_ENTITY(entity, members)    \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members)       \
	MONETA_ENTITY_NAME(entity, entity)                    \
	MONETA_PP_EXPAND_ENTITY_MEMBERS(entity, members)      \
	MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(entity, members)

#define MONETA_DEFINE_ENTITY_STRUCT_WITH_NAMESPACE(ns_seq, entity, members) \
	MONETA_PP_NAMESPACE_BEGIN(ns_seq)                                   \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members)                     \
	MONETA_PP_NAMESPACE_END(ns_seq)
