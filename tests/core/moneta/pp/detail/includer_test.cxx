// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"

// TODO: Investigate further why this header needs to be fully qualified.
#define INCLUDES ("moneta/pp/detail/includer_test_header.hxx")
#include <moneta/pp/detail/includer.hxx>

#ifndef TEST_SUCCESS
	// Error: Our test header "includer_test_header.hxx" failed to be included.
	BOOST_STATIC_ASSERT(boost::false_type);
#endif
