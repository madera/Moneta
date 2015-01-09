#pragma once
#include "../traits/entity_name.hxx"
#include "../traits/member_names.hxx"
#include "traits/table_name.hxx"
#include "traits/field_names.hxx"

// ---

#define MONETA_PP_MEMBER_FROM_TNT_TRIO(entity, trio) \
	MONETA_MEMBER(entity, BOOST_PP_TUPLE_ELEM(3, 0, trio), BOOST_PP_TUPLE_ELEM(3, 1, trio))

// ---

#define __MONETA_PP_EXPAND_SQL_ENTITY_MEMBER_NAMES(r, entity, trio)                                \
	namespace moneta { namespace traits { namespace detail {                                   \
		template <>                                                                        \
		struct member_name<MONETA_PP_MEMBER_FROM_TNT_TRIO(entity, trio)> : boost::true_type { \
			typedef std::string trait_type;                                            \
			static trait_type get() {                                                  \
				return BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, 1, trio));        \
			}                                                                          \
		};                                                                                 \
	}}}

#define MONETA_PP_EXPAND_SQL_ENTITY_MEMBER_NAMES(entity, members) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_SQL_ENTITY_MEMBER_NAMES, entity, members)

// ---

#define __MONETA_PP_EXPAND_SQL_ENTITY_MEMBERS(r, entity, trio) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(r, 2)) MONETA_PP_MEMBER_FROM_TNT_TRIO(entity, trio)

#define MONETA_PP_EXPAND_SQL_ENTITY_MEMBERS(entity, members)                                         \
	namespace moneta { namespace traits { namespace detail {                                      \
		template <>                                                                           \
		struct members_of<entity> : boost::mpl::vector<                                       \
			BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_SQL_ENTITY_MEMBERS, entity, members) \
		>{};                                                                                  \
	}}}                                                                                           \

// ---

#define __MONETA_PP_EXPAND_SQL_ENTITY_FIELD_NAMES(r, entity, trio)                                    \
	namespace moneta { namespace traits { namespace detail {                                      \
		template <>                                                                           \
		struct sql_field_name<MONETA_PP_MEMBER_FROM_TNT_TRIO(entity, trio)> : boost::true_type { \
			typedef std::string trait_type;                                               \
			static trait_type get() {                                                     \
				return BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, 2, trio));           \
			}                                                                             \
		};                                                                                    \
	}}}

#define MONETA_PP_EXPAND_SQL_ENTITY_FIELD_NAMES(entity, members) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_SQL_ENTITY_FIELD_NAMES, entity, members)

// ---

#define MONETA_DESCRIBE_SQL_ENTITY(entity, table, members)        \
	MONETA_ENTITY_NAME(entity, entity)                        \
	MONETA_SQL_TABLE_NAME(entity, table)                      \
	MONETA_PP_EXPAND_SQL_ENTITY_MEMBERS(entity, members)     \
	MONETA_PP_EXPAND_SQL_ENTITY_MEMBER_NAMES(entity, members) \
	MONETA_PP_EXPAND_SQL_ENTITY_FIELD_NAMES(entity, members)
//	MONETA_PP_EXPAND_PRIMARY_KEYS(entity, members) // XXX: Fix this maybe?
