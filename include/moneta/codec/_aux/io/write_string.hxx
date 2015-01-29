#pragma once
#include <iterator>

namespace moneta { namespace codec {

	namespace io {

		template <typename OutputIterator>
		int write(
			OutputIterator& next, OutputIterator end,
			const char* cstr,
			const bool zero_terminate = true
		) {
			int consumed = 0;

			while (next != end && *cstr != 0) {
				*next++ = *cstr++;
				++consumed;
			}

			// Ran out of space and there's still data?
			if (next == end) {
				if (*cstr != 0 || zero_terminate) {
					int left = strlen(cstr);
					if (zero_terminate) {
						++left;
					}

					return 0 - left;
				}
			}

			if (zero_terminate && next != end) {
				*next++ = 0;
				++consumed;
			}

			return consumed;
		}

	}

}}
