#pragma once
#include "members.hxx"
#include "detail/member_trait_base.hxx"
#include "detail/has_member_trait.hxx"
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/back_inserter.hpp>

#include <boost/preprocessor/tuple/push_back.hpp>

MONETA_DECLARE_TRAIT(fixed_value)

#define MONETA_FIXED_VALUE(member, value) \
	MONETA_DEFINE_TRAIT_WITH_GET(fixed_value, member, (__UNWRAP_TUPLE(member)::result_type), value)

#define MONETA_FIXED_VALUE_MEMBER(entity, type, name, value) \
	MONETA_FIXED_VALUE(MONETA_MEMBER(entity, type, name), value)

namespace moneta { namespace traits {

	template <class Entity>
	struct fixed_value_members : boost::mpl::copy_if<
		typename traits::members<Entity>::type,
		traits::detail::fixed_value<boost::mpl::_1>,
		boost::mpl::back_inserter<boost::mpl::vector<> >
	> {};

}}
