#pragma once
#include "../traits/detail/member_trait_base.hxx"
#include "members_of.hxx"
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include <vector>
#include <string>

MONETA_DECLARE_MEMBER_TRAIT(member_name, std::string)

#define MONETA_MEMBER_NAME(member, name) \
	MONETA_DEFINE_MEMBER_TRAIT(member_name, member, std::string, name)

namespace moneta { namespace meta {

	template <class EntityType>
	std::vector<std::string> get_member_names() {
		std::vector<std::string> result;

		boost::fusion::for_each(
			typename members_of<EntityType>::type(),
			traits::detail::trait_back_inserter<traits::detail::member_name>(result)
		);

		return result;
	}

}}
