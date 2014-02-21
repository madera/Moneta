#pragma once
#include "members_of.hxx"
#include "member_name.hxx"
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include <vector>
#include <string>

namespace moneta { namespace meta {

	template <class EntityType>
	std::vector<std::string> get_member_names() {
		std::vector<std::string> result;

		boost::fusion::for_each(
			typename members_of<EntityType>::type(),
			detail::make_trait_back_inserter<detail::member_name>(result)
		);

		return result;
	}

}}
