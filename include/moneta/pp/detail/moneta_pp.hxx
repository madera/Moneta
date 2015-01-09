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
