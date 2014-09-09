#pragma once
#include "core.hxx"
#include "detail/kv.hxx"
#include "detail/seq_join.hxx"

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

//
// Accessors
//
#define MONETA_PP_ENTITY_NAME(data)      PP_KVS_VALUE(data, ENTITY_NAME)
#define MONETA_PP_ENTITY_NAMESPACE(data) PP_KVS_VALUE(data, ENTITY_NAMESPACE)
#define MONETA_PP_ENTITY_MEMBERS(data)   PP_KVS_VALUE(data, ENTITY_MEMBERS)

//
// Constructors
//
#define MONETA_PP_DEFAULT_CONSTRUCTOR(name) name() {}

#define __CTOR_PARAMS_LIST(z, n, data) \
	BOOST_PP_COMMA_IF(n) \
	BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(0, data)) \
	BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1, data)), _)
#define CTOR_PARAMS_LIST(types, names) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(types), __CTOR_PARAMS_LIST, (types)(names))

#define __CTOR_INIT_LIST(z, n, names) BOOST_PP_COMMA_IF(n) \
	BOOST_PP_SEQ_ELEM(n, names) (BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(n, names), _))
#define CTOR_INIT_LIST(names) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(names), __CTOR_INIT_LIST, names)

#define MONETA_PP_PARAMS_CONSTRUCTOR(name, types, names) \
	name(CTOR_PARAMS_LIST(types, names)) : (CTOR_INIT_LIST(names)) {} // XXX: Remove parentheses after testing.

//
// Members
//
#define MONETA_PP_MEMBER_DECLARATIONS(datatypes, names) BOOSTX_PP_SEQ_JOIN( PP_SEQ_JOIN_2(datatypes, names), ;) ;
