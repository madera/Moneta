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

#pragma once
#include <boost/config.hpp>
#include <boost/config/suffix.hpp>

#ifdef BOOST_MSVC
#  define MONETA_INTRA_TEMPLATE_KEYWORD
#else
#  define MONETA_INTRA_TEMPLATE_KEYWORD template
#endif

// TODO: Kill MONETA_INTRA_TEMPLATE_KEYWORD.
#define MONETA_DEDUCED_TYPENAME MONETA_INTRA_TEMPLATE_KEYWORD
