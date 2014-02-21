#pragma once
#include "members_of.hxx"
#include "member_name.hxx"
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include <vector>
#include <string>

namespace moneta { namespace meta {

	namespace detail {
		template <
			template <typename T>
			class MemberTraitWithGet,

			typename ContainerType = std::vector<
				typename MemberTraitWithGet<not_a_member>::trait_type
			>
		>
		struct trait_back_inserter  {
			ContainerType& _target;

			trait_back_inserter(ContainerType& target)
			 : _target(target) {
			}

			template <class MemberType>
			void operator()(MemberType& member) const {
				_target.push_back(MemberTraitWithGet<MemberType>::get());
			}
		};
	}

	template <class EntityType>
	std::vector<std::string> get_member_names() {
		std::vector<std::string> result;

		//boost::fusion::for_each(
		//	typename members_of<EntityType>::type(),
		//	detail::trait_back_inserter<
		//		detail::member_name
		//		//, std::vector<std::string>
		//	>(result)
		//);

		boost::fusion::for_each(
			typename members_of<EntityType>::type(),
			detail::trait_back_inserter<detail::member_name, std::vector<std::string> >(result)
		);

		return result;
	}

}}
