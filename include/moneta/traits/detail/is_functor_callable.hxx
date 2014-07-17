#pragma once

namespace moneta { namespace traits { namespace detail {

#define IS_METHOD_CALLABLE_NAME is_functor_callable
#define IS_METHOD_CALLABLE_FUNCTION operator()
#include "pp/is_method_callable_spec.hxx"

}}}
