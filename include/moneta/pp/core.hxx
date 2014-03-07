#pragma once
#include "detail/boostx.hxx"
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#include "moneta_pp.hxx"

#define ENTITY_NAMESPACE 1
#define ENTITY_NAME      2
#define ENTITY_MEMBERS   3
#define ENTITY_MEMBER    4

#define MEMBER_DATATYPE  100
#define MEMBER_NAME      101

//
// User Macros
//
#define NAMESPACE(ns) ((ENTITY_NAMESPACE) (ns))
#define NAME(name) ((ENTITY_NAME) (name))

#define FQN(fqn) NAMESPACE(BOOST_PP_SEQ_POP_BACK(fqn)) NAME(BOOSTX_PP_SEQ_LAST(fqn))

#define MEMBERS_BEGIN ((ENTITY_MEMBERS)(
#define MEMBERS_END ))

#define MEMBER_BEGIN ((ENTITY_MEMBER)(
#define MEMBER_END ))

#define MEMBER(type, name) MEMBER_BEGIN \
		( \
			(MEMBER_DATATYPE)(type) \
		) \
		( \
			(MEMBER_NAME)(name) \
		) \
	MEMBER_END

#define SQL_MEMBER(type, name, field) MEMBER_BEGIN \
	(\
	(MEMBER_DATATYPE)(type) \
	) \
	(\
	(MEMBER_NAME)(name) \
	) \
	MEMBER_END
