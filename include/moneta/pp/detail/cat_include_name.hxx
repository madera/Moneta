#pragma once
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/elem.hpp>

#define MONETA_PP_CAT_INCLUDE_NAME_1(SEQ) BOOST_PP_SEQ_ELEM(0, SEQ)
#define MONETA_PP_CAT_INCLUDE_NAME_2(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_1(SEQ),/), BOOST_PP_SEQ_ELEM(1, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_3(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_2(SEQ),/), BOOST_PP_SEQ_ELEM(2, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_4(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_3(SEQ),/), BOOST_PP_SEQ_ELEM(3, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_5(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_4(SEQ),/), BOOST_PP_SEQ_ELEM(4, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_6(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_5(SEQ),/), BOOST_PP_SEQ_ELEM(5, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_7(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_6(SEQ),/), BOOST_PP_SEQ_ELEM(6, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_8(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_7(SEQ),/), BOOST_PP_SEQ_ELEM(7, SEQ))
#define MONETA_PP_CAT_INCLUDE_NAME_9(SEQ) BOOST_PP_CAT(BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_8(SEQ),/), BOOST_PP_SEQ_ELEM(8, SEQ))

#define MONETA_PP_CAT_INCLUDE_NAME(SEQ) \
	BOOST_PP_CAT(MONETA_PP_CAT_INCLUDE_NAME_, BOOST_PP_SEQ_SIZE(SEQ))(SEQ)
