#include "stdafx.h"

#define INCLUDES ("includer_test_header.hxx")
#include <moneta/pp/detail/includer.hxx>

static void static_test() {
#ifndef TEST_SUCCESS
	// Error: Our test header "includer_test_header.hxx" failed to be included.
	BOOST_STATIC_ASSERT(boost::false_type);
#endif
}

#include "../../model/Address.hxx"
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

//#pragma message(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, (A, B, C, D))))

#define MEMS \
	(int,         ID) \
	(int,         Number) \
	(std::string, Street)


//#pragma message(BOOST_PP_STRINGIZE((MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES(Address, MEMS))))

#define ADD_PAR(ATTRIBUTES) BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_FILLER_0 ATTRIBUTES, _END)

#define MACRO(a, b, c) [a|b|c]

#define MACRO2(a, b, pair) MONETA_MEMBER_NAME(MONETA_MEMBER_FROM_TN_PAIR(_, pair), BOOST_PP_TUPLE_ELEM(2, 1, pair))

#define X BOOST_PP_SEQ_FOR_EACH(MACRO2, _, ADD_PAR(MEMS))
#define Y BOOST_PP_SEQ_FOR_EACH(__MONETA_PP_EXPAND_ENTITY_MEMBER_NAMES, _, ADD_PAR(MEMS))
#pragma message(BOOST_PP_STRINGIZE((X)))
