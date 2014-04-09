#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../traits/rtuple.hxx"
#include "../traits/to_rtuple.hxx"
#include "../traits/to_rtie.hxx"
#include <bitset>

namespace moneta { namespace container {

	template <class EntityType>
	struct null_load_tracker {
		null_load_tracker() {
		}

		null_load_tracker(const bool ignored) {
		}

		const bool loaded(const size_t index) const {
			return true;
		}

		void loaded(const size_t, const bool value) {
		}
	};

}}
