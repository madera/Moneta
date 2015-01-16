#pragma once
#include "tabs.hxx"

namespace moneta { namespace codec { namespace aux {

	namespace detail {

		template <class Path, signed int Extra = 0>
		struct tabs_impl {
			static const char* get() {
				const signed int n = boost::mpl::size<Path>::value +
					      moneta::algorithm::detail::container_member_count<Path>::value + 
					      Extra;
				
				const int count = (n >= 0)? n : 0;
				return tabs<count % MONETA_CODEC_TABS_MAX>::get();
			}
		};

	}

	template <class Path>
	inline const char* path_tabs() {
		return detail::tabs_impl<
			Path,
			moneta::algorithm::detail::is_cwd_container_member<Path>::type::value
		>::get();
	}

	template <class Path, int Extra>
	inline const char* path_tabs() {
		return detail::tabs_impl<
			Path,
			Extra + moneta::algorithm::detail::is_cwd_container_member<Path>::type::value
		>::get();
	}

}}}
