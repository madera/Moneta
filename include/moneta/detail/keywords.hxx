#pragma once
#include <boost/config.hpp>

#ifdef BOOST_MSVC
#define MONETA_INTRA_TEMPLATE_KEYWORD
#else
#define MONETA_INTRA_TEMPLATE_KEYWORD template
#endif
