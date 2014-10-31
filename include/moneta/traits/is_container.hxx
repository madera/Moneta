#pragma once

//
// TODO: Will this stand the test of time?
//
// Currently we borrow the code from Boost Spirit. In the future, we may need to reimplement it.
//
#include <boost/spirit/home/support/container.hpp>

namespace moneta { namespace traits {

	using boost::spirit::traits::is_container;

}}
