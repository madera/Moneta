#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../sql/traits/rtuple.hxx"
#include "../sql/traits/to_db_tuple.hxx"
#include "../sql/traits/to_db_tie.hxx"
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
