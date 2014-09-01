// TODO: Boost.PP this file.
#pragma once
#include "../../traits/members.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/mpl/size.hpp>

#define __IMPL_TOKEN__(z, n, text) \
	else if (index == n) { \
		typedef typename at_c<typename traits::members<Entity>::type, n>::type member_type; \
		member_type()(result) = boost::lexical_cast<typename member_type::result_type>(value); \
	}

#define PP_DEFINE_IMPL(n) \
	template <class Entity> \
	struct from_text_impl<Entity, n> { \
		template <class Entity> \
		void operator()(Entity& result, const size_t index, const std::string& value) const { \
			using boost::mpl::at_c; \
			if (false) {} \
			BOOST_PP_REPEAT(n, __IMPL_TOKEN__, _) \
		} \
	};

namespace moneta { namespace serialization { namespace detail {

	template <class Entity, int Size = boost::mpl::size<traits::members<Entity>::type>::value>
	struct from_text_impl;

	template <class Entity>
	struct from_text_impl<Entity, 0> {
		template <class Entity>
		void operator()(Entity& result, const size_t index, const std::string& value) const {
		}
	};

	// XXX: Loop automatically here.
	PP_DEFINE_IMPL(1)
	PP_DEFINE_IMPL(2)
	PP_DEFINE_IMPL(3)
	PP_DEFINE_IMPL(4)
	PP_DEFINE_IMPL(5)
	PP_DEFINE_IMPL(6)
	PP_DEFINE_IMPL(7)
	PP_DEFINE_IMPL(8)
	PP_DEFINE_IMPL(9)
	PP_DEFINE_IMPL(10)

}}}

#undef PP_DEFINE_IMPL
#undef __IMPL_TOKEN__
