#include "stdafx.h"

// TODO: Investigate further why this header needs to be fully qualified.
#define INCLUDES ("moneta/pp/detail/includer_test_header.hxx")
#include <moneta/pp/detail/includer.hxx>

#ifndef TEST_SUCCESS
	// Error: Our test header "includer_test_header.hxx" failed to be included.
	BOOST_STATIC_ASSERT(boost::false_type);
#endif
