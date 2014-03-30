#pragma once
#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../../sql/traits/rtuple.hxx"
#include "../../sql/traits/to_rtuple.hxx"
#include "../../sql/traits/to_rtie.hxx"
#include <bitset>

namespace moneta { namespace container {

	template <class EntityType>
	struct bitset_load_tracker {
		typedef typename std::bitset<
			boost::mpl::size<
				typename moneta::sql::traits::rtuple<EntityType>::type
			>::value
		> state_type;

	public:
		state_type load_state;

	public:
		bitset_load_tracker() {
		}

		bitset_load_tracker(const bool all_loaded) {
			if (all_loaded) {
				load_state.set();
			}
		}

		const bool loaded(const size_t ordinal) const {
			return load_state[ordinal];
		}

		void loaded(const size_t ordinal, const bool value) {
			load_state[ordinal] = value;
		}
	};

}}
