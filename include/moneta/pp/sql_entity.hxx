//
// TODO: Rebase this file on the new directory structure, TBD.
//
#pragma once
#include "core.hxx"
#include <moneta/traits/pk.hxx>
#include <moneta/traits/members.hxx>
#include <moneta/traits/member.hxx>
#include <moneta/traits/member_names.hxx>
#include <moneta/traits/entity_name.hxx>
#include <moneta/sql/traits/table_name.hxx>
#include <moneta/sql/traits/field_names.hxx>
#include <boost/mpl/vector.hpp>

#define __MONETA_PP_EXPAND_ENTITY_SQL_FIELD_NAMES(r, _, pair) \
	MONETA_SQL_FIELD_NAME(MONETA_MEMBER_FROM_TN_PAIR(_, pair), BOOST_PP_TUPLE_ELEM(3, 2, pair))

#define MONETA_PP_EXPAND_ENTITY_SQL_FIELD_NAMES(entity, member_sequence) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_ENTITY_SQL_FIELD_NAMES, entity, member_sequence)

#define PROCESS_SQL_4(entity, tuple) \
	BOOST_PP_IIF(\
		BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(4, 3, tuple), MONETA_PRIMARY_KEY), \
		MONETA_DECLARE_PRIMARY_KEY, \
		BOOST_PP_TUPLE_EAT_3 \
	)(entity, BOOST_PP_TUPLE_ELEM(4, 0, tuple), BOOST_PP_TUPLE_ELEM(4, 1, tuple))

#define __MONETA_PP_EXPAND_PRIMARY_KEYS(r, entity, tuple) \
	BOOST_PP_IIF( \
		BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(tuple), 4), \
		PROCESS_SQL_4, \
		BOOST_PP_TUPLE_EAT(BOOST_PP_TUPLE_SIZE(tuple)) \
	)(entity, tuple)

#define MONETA_PP_EXPAND_PRIMARY_KEYS(entity, members) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_PRIMARY_KEYS, entity, members)

#define MONETA_DESCRIBE_SQL_ENTITY(entity, table, members) \
	MONETA_DESCRIBE_ENTITY_BASE(entity, members) \
	MONETA_ENTITY_NAME(entity, entity) \
	MONETA_SQL_TABLE_NAME(entity, table) \
	MONETA_PP_EXPAND_ENTITY_SQL_FIELD_NAMES(entity, members)
//	MONETA_PP_EXPAND_PRIMARY_KEYS(entity, members)

#define MONETA_DEFINE_AND_DESCRIBE_SQL_ENTITY(entity, table, members) \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members) \
	MONETA_DESCRIBE_SQL_ENTITY(entity, table, members)

#define MONETA_DESCRIBE_ENTITY_NG(entity, members) \
	MONETA_DESCRIBE_ENTITY_BASE(entity, members) \
	MONETA_ENTITY_NAME(entity, entity)

#define MONETA_DEFINE_AND_DESCRIBE_ENTITY(entity, members) \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members) \
	MONETA_DESCRIBE_ENTITY_NG(entity, members)
