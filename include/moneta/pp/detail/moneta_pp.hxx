#pragma once
#include "add_parentheses.hxx"
#include "boostx.hxx"
#include "seq_join.hxx"
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/size.hpp>

// XXX: Move.
#if BOOST_PP_VARIADICS
#define __UNWRAP_TUPLE(t) BOOST_PP_TUPLE_REM(BOOST_PP_TUPLE_SIZE(t)) t
#else
//#error No Variadic support
#endif

//
// Namespaces
//
#define __MONETA_PP_NAMESPACE_BEGIN(r, data, t) namespace t {
#define MONETA_PP_NAMESPACE_BEGIN(ns_seq) BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_NAMESPACE_BEGIN, _, ns_seq)

#define __MONETA_PP_NAMESPACE_END(r, data, t) }
#define MONETA_PP_NAMESPACE_END(ns_seq) BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_NAMESPACE_END, _, ns_seq)

// Usage: MONETA_PP_NAMESPACE_JOIN((a)(b)(c), ::) --> a::b::c::
#define __MONETA_PP_NAMESPACE_JOIN(r, data, t) t data
#define MONETA_PP_NAMESPACE_JOIN(ns_seq, token) BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_NAMESPACE_JOIN,token,ns_seq)

// --------------------------------------------------------------------------------------------------------------------

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
