#pragma once

#include <boost/config.hpp>

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50
#define FUSION_MAX_VECTOR_SIZE 50	

#ifdef BOOST_WINDOWS
#include "targetver.h"
#endif

//
// Boost
//
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

//
// Std
//
#include <iostream>
