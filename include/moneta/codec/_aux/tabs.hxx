#pragma once

namespace moneta { namespace codec { namespace aux {

	#define MONETA_CODEC_TABS_MAX 32

	namespace {
		const char* tabs_cstr = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	}

	template <int N>
	struct tabs {
		enum  { size = MONETA_CODEC_TABS_MAX };
		static const char* get() {
			return &tabs_cstr[size - (N % size)];
		}
	};

}}}
