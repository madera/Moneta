#pragma once

namespace moneta { namespace codec { namespace aux {

	#define MONETA_CODEC_TABS_MAX 32

	namespace {
		const char* tabs_cstr = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	}

	template <signed int N>
	struct tabs {
		enum  { size = MONETA_CODEC_TABS_MAX };
		static const char* get() {
			const size_t count = (N > 0)? N : 0;
			return &tabs_cstr[size - (count % size)];
		}
	};

}}}
