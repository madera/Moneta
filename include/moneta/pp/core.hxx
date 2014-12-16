#pragma once
#include "moneta_pp.hxx"
#include "detail/add_parentheses.hxx"
#include "detail/boostx.hxx"
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#define ENTITY_NAMESPACE 1
#define ENTITY_NAME      2
#define ENTITY_MEMBERS   3
#define ENTITY_MEMBER    4

#define MEMBER_DATATYPE  100
#define MEMBER_NAME      101

#define MONETA_PRIMARY_KEY 137

// XXX: Move.
#define __WRAP_MACRO_PARAM(x) (x)
#define __UNWRAP_MACRO_PARAM(x) x

#define __UNWRAP_TUPLE(t) BOOST_PP_TUPLE_REM(BOOST_PP_TUPLE_SIZE(t)) t

//
// User Macros
//
#define NAMESPACE(ns) ((ENTITY_NAMESPACE) (ns  ))
#define NAME(name)    ((ENTITY_NAME     ) (name))

#define FQN(fqn) NAMESPACE(BOOST_PP_SEQ_POP_BACK(fqn)) NAME(BOOSTX_PP_SEQ_LAST(fqn))

#define MEMBERS_BEGIN ((ENTITY_MEMBERS)(
#define MEMBERS_END   ))

#define MEMBER_BEGIN ((ENTITY_MEMBER)(
#define MEMBER_END   ))

#define MEMBER(type, name) \
	MEMBER_BEGIN \
	( (MEMBER_DATATYPE)(type) ) \
	( (MEMBER_NAME)    (name) ) \
	MEMBER_END

#define SQL_MEMBER_FIELD_NAME 200

#define SQL_MEMBER(type, name, field) \
	MEMBER_BEGIN \
	( (MEMBER_DATATYPE)      (type)  ) \
	( (MEMBER_NAME)          (name)  ) \
	( (SQL_MEMBER_FIELD_NAME)(field) ) \
	MEMBER_END

//
// XXX: Move somewhere else!!
//

#define MONETA_MEMBER_FROM_TN_PAIR(entity, pair) \
	MONETA_MEMBER(entity, BOOST_PP_TUPLE_ELEM(2, 0, pair), BOOST_PP_TUPLE_ELEM(2, 1, pair))

#define __MONETA_PP_EXPAND_ENTITY_MEMBERS(r, _, pair) \
	BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(r, 2)) MONETA_MEMBER_FROM_TN_PAIR(_, pair)
#define MONETA_PP_EXPAND_ENTITY_MEMBERS(entity, member_sequence) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_ENTITY_MEMBERS, entity, member_sequence)

#define __MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(r, _, pair) \
	MONETA_MEMBER_NAME(MONETA_MEMBER_FROM_TN_PAIR(_, pair), BOOST_PP_TUPLE_ELEM(2, 1, pair))
#define MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(entity, member_sequence) \
	BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES, entity, member_sequence)



#define MONETA_DESCRIBE_ENTITY_BASE(entity, members) \
	namespace moneta { namespace traits { namespace detail { \
		template <> \
		struct members_of<entity> : boost::mpl::vector< \
			MONETA_PP_EXPAND_ENTITY_MEMBERS(entity, members) \
		>{}; \
	}}} \
	MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(entity, members)

//#define MONETA_DESCRIBE_ENTITY(entity, members) \
//	MONETA_DESCRIBE_ENTITY_BASE(entity, MONETA_PP_ADD_PARENTHESES_2(members))

//
// XXX: New interface...
//
#define __MONETA_PP_DEFINE_MEMBERS(r, _, tuple) BOOST_PP_TUPLE_ELEM(2, 0, tuple) BOOST_PP_TUPLE_ELEM(2, 1, tuple);
#define MONETA_PP_DEFINE_MEMBERS(members) BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_DEFINE_MEMBERS, _, members)

#define MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members) \
	struct entity { \
		MONETA_PP_DEFINE_MEMBERS(members) \
	};

#define MONETA_PP_DEFINE_ENTITY_STRUCT_WITH_NAMESPACE(ns_seq, entity, members) \
	MONETA_PP_NAMESPACE_BEGIN(ns_seq) \
	struct entity { \
		MONETA_PP_DEFINE_MEMBERS(members) \
	}; \
	MONETA_PP_NAMESPACE_END(ns_seq)
