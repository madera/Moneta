#pragma once
#include <boost/multi_index/member.hpp> // We reuse boost::multi_index::member<>

namespace moneta { namespace traits {

	template<class Class, typename Type, Type Class::* PtrToMember>
	struct member : boost::multi_index::member<Class, Type, PtrToMember> {
		typedef Class class_type;

		static Type Class::* get() {
			return PtrToMember;
		}
	};

	struct not_a_member {};
}}

#define MONETA_MEMBER(k, r, n) moneta::traits::member<k, r, &k::n>
