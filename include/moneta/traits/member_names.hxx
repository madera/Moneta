// TODO: Rename to member_names.hxx
#pragma once
#include "detail/member_trait_base.hxx"
#include "members.hxx"
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include <vector>
#include <string>

MONETA_DECLARE_MEMBER_TRAIT(member_name, std::string)

#define MONETA_MEMBER_NAME(member, name) \
	MONETA_DEFINE_MEMBER_TRAIT(member_name, member, std::string, name)

namespace moneta { namespace traits {

	namespace detail {
		template <class MemberPointersSequence>
		std::vector<std::string> get_memptr_names() {
			std::vector<std::string> result;
			boost::fusion::for_each(
				typename MemberPointersSequence(),
				detail::trait_back_inserter<detail::member_name>(result)
			);

			return result;
		}
	}

	template <class EntityType>
	std::vector<std::string> get_member_names() {
		return detail::get_memptr_names<members<EntityType>::type>();
	}

}}
