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
