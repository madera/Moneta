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

#include "pp/is_method_callable_spec.hxx"
DEFINE_IS_CALL_POSSIBLE(is_functor_callable, operator())

//#include <boost/type_traits/add_reference.hpp> // XXX: Check the necessity of this...
//
//namespace moneta { namespace traits { namespace detail {
//
//#define IS_METHOD_CALLABLE_NAME is_functor_callable
//#define IS_METHOD_CALLABLE_FUNCTION operator()
//#include "pp/is_method_callable_spec.hxx"
//
//}}}
