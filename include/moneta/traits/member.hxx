#pragma once
#include <boost/multi_index/member.hpp> // We reuse boost::multi_index::member<>

namespace moneta { namespace traits {

	template <class K, typename R, R K::* memptr>
	struct member : boost::multi_index::member<K, R, memptr> {
		typedef K class_type;

		static R K::* get() {
			return memptr;
		}
	};

	struct not_a_member {};
}}

#define MONETA_MEMBER(k, r, n) moneta::traits::member<k, r, &k::n>
