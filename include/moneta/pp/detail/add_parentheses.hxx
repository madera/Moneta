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

// TODO: Continue this Joel-de-Guzman-inspired charm.

#pragma once
#include <boost/preprocessor/tuple/size.hpp>

#define MONETA_PP_ADD_PARENTHESES_2_1(A, B) ((A, B)) MONETA_PP_ADD_PARENTHESES_2_2
#define MONETA_PP_ADD_PARENTHESES_2_2(A, B) ((A, B)) MONETA_PP_ADD_PARENTHESES_2_1
#define MONETA_PP_ADD_PARENTHESES_2_1_END
#define MONETA_PP_ADD_PARENTHESES_2_2_END
#define MONETA_PP_ADD_PARENTHESES_2(S) BOOST_PP_CAT(MONETA_PP_ADD_PARENTHESES_2_1 S, _END)

#define MONETA_PP_ADD_PARENTHESES_3_1(A, B, C) ((A, B, C)) MONETA_PP_ADD_PARENTHESES_3_2
#define MONETA_PP_ADD_PARENTHESES_3_2(A, B, C) ((A, B, C)) MONETA_PP_ADD_PARENTHESES_3_1
#define MONETA_PP_ADD_PARENTHESES_3_1_END
#define MONETA_PP_ADD_PARENTHESES_3_2_END
#define MONETA_PP_ADD_PARENTHESES_3(S) BOOST_PP_CAT(MONETA_PP_ADD_PARENTHESES_3_1 S, _END)
