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

#define __PP_SEQ_JOIN_2(z, n, data) ( \
	BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(0, data)) \
	BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1, data)) \
)
#define PP_SEQ_JOIN_2(s0, s1) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(s0), __PP_SEQ_JOIN_2, (s0)(s1))

#define __PP_SEQ_JOIN_3(z, n, data) ( \
	BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(0, data)) \
	BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1, data)) \
	BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(2, data)) \
)
#define PP_SEQ_JOIN_3(s0, s1, s2) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(s0), __PP_SEQ_JOIN_3, (s0)(s1)(s2))
