#pragma once
#include "members_of.hxx"
#include "member_name.hxx"
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include <vector>
#include <string>

namespace moneta { namespace meta {

	namespace detail {
		struct field_name_back_inserter  {
			std::vector<std::string>& fields;

			field_name_back_inserter(std::vector<std::string>& v)
				: fields(v) {
			}

			template <class MemberType>
			void operator()(MemberType& member) const {
				fields.push_back(member_name<MemberType>::get());
			}
		};
	}

	template <class EntityType>
	std::vector<std::string> get_member_names() {
		
		std::vector<std::string> result;
		boost::fusion::for_each(
			typename members_of<EntityType>::type(),
			detail::field_name_back_inserter(result)
		);

		return result;
	}

}}
