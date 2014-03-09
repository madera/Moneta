#include "stdafx.h"

#define INCLUDES ("includer_test_header.hxx")
#include <moneta/pp/detail/includer.hxx>

static void static_test() {
#ifndef TEST_SUCCESS
	// Error: Our test header "includer_test_header.hxx" failed to be included.
	BOOST_STATIC_ASSERT(boost::false_type);
#endif
}
