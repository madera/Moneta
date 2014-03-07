#pragma once
#include "boostx.hxx"
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

//
// Misc.
//
#define PP_KV_CALL(kv_op, seq) kv_op(BOOST_PP_SEQ_ELEM(0, seq), BOOST_PP_SEQ_ELEM(1, seq))

//
// PP_KVS_VALUE
// Get a value from a KV Sequence.
//
// Example:
//
// (
//	(a)(b)
// )
// (
//	(c)(d)
// )
//
// PP_KVS_VALUE(data, a) -> b
// PP_KVS_VALUE(data, c) -> d
//
#define __PP_KVS_VALUE_TOKEN(r, data, tag) \
	BOOST_PP_IF( \
		BOOST_PP_EQUAL(BOOST_PP_SEQ_ELEM(0, tag), data), \
		BOOST_PP_SEQ_ELEM(1, tag), \
		BOOST_PP_EMPTY() \
	)
#define PP_KVS_VALUE(data, tag) BOOST_PP_SEQ_FOR_EACH_R(1, __PP_KVS_VALUE_TOKEN, tag, data)

//
// .
//
#define __PP_KVS_VALUES(r, data, tag) \
	BOOST_PP_IF( \
		BOOST_PP_EQUAL(BOOST_PP_SEQ_ELEM(0, tag), data), \
		( BOOST_PP_SEQ_ELEM(1, tag) ), \
		BOOST_PP_EMPTY() \
	)
#define PP_KVS_VALUES(data, tag) BOOST_PP_SEQ_FOR_EACH_R(1, __PP_KVS_VALUES, tag, data)

//
// PP_KVS_VALUE
// Get a value from a KV Sequence path.
//
#define __PP_KVS_PATH_VALUE(s, state, x) PP_KVS_VALUE(state, x)
#define PP_KVS_PATH_VALUE(data, ns_seq) BOOST_PP_SEQ_FOLD_LEFT(__PP_KVS_PATH_VALUE, data, ns_seq)

//
// PP_KVS_FOREACH
//
#define __PP_KVS_FOREACH(r, kv_op, tag) kv_op(BOOST_PP_SEQ_ELEM(0, tag), BOOST_PP_SEQ_ELEM(1, tag))
#define PP_KVS_FOREACH(kv_op, data) BOOST_PP_SEQ_FOR_EACH_R(1, __PP_KVS_FOREACH, kv_op, data)

//
// PP_KVS_PATH_FOREACH
//
#define PP_KVS_PATH_FOREACH(kv_op, data, ns_seq) PP_KVS_FOREACH(kv_op, PP_KVS_PATH_VALUE(data, ns_seq))
