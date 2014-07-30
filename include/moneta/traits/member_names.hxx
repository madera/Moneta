#pragma once
#include "detail/member_trait_base.hxx"

// TODO: RENAME THIS FILE TO SINGULAR FORM

MONETA_DECLARE_MEMBER_TRAIT(member_name)

#define MONETA_MEMBER_NAME(member, name) \
	MONETA_DEFINE_TRAIT_WITH_GET(member_name, member, std::string, BOOST_PP_STRINGIZE(name))

namespace moneta { namespace traits {

	MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(member_name, std::string, get_member_names)

	// FIXME: O(n): Sloooowwww.
	template <class EntityType>
	const size_t get_member_name_index(const std::string& name) {
		const std::vector<std::string>& data = get_member_names<EntityType>();

		std::vector<std::string>::const_iterator itr = std::find(data.begin(), data.end(), name);
		if (itr == data.end()) {
			assert(false);
			return -1;
		}

		return std::distance(data.begin(), itr);
	}

}}
