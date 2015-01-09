#ifndef BOOST_PP_IS_ITERATING
#    ifndef MONETA_INCLUDER_HEADER
#    define MONETA_INCLUDER_HEADER
#        include <boost/preprocessor/iteration/iterate.hpp>
#        include <boost/preprocessor/stringize.hpp>
#        include <boost/preprocessor/cat.hpp>
#        include <boost/preprocessor/seq/size.hpp>
#        define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(INCLUDES)))
#        define BOOST_PP_FILENAME_1 "moneta/pp/detail/includer.hxx"
#        include BOOST_PP_ITERATE()
#    endif
#else
#pragma message("Including [" BOOST_PP_STRINGIZE(BOOST_PP_ITERATION()) "]: " \
	BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(BOOST_PP_ITERATION(), INCLUDES)))
#include BOOST_PP_SEQ_ELEM(BOOST_PP_ITERATION(), INCLUDES)
#endif
