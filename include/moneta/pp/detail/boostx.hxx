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

#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

//
// BOOSTX
//
// Some helper Boost PP extensions.
//

// BOOSTX_PP_SEQ_JOIN: Join sequence.
#define BOOSTX_PP_SEQ_JOIN_TOKEN(r, data, t) t BOOST_PP_IF(BOOST_PP_EQUAL(r, BOOST_PP_INC(BOOST_PP_SEQ_ELEM(0, data))), BOOST_PP_EMPTY(), BOOST_PP_SEQ_ELEM(1, data))
#define BOOSTX_PP_SEQ_JOIN(seq, sep) BOOST_PP_SEQ_FOR_EACH(BOOSTX_PP_SEQ_JOIN_TOKEN, (BOOST_PP_SEQ_SIZE(seq))(sep), seq)

// BOOSTX_PP_SEQ_LAST: Get last element of a sequence.
#define BOOSTX_PP_SEQ_LAST(seq) BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), seq)
