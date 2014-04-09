#pragma once
#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../../traits/rtuple.hxx"
#include "../../traits/to_rtuple.hxx"
#include "../../traits/to_rtie.hxx"
#include <bitset>

namespace moneta { namespace container {

	template <class EntityType>
	struct bitset_change_tracker {
		typedef typename std::bitset<
			boost::mpl::size<
				typename moneta::sql::traits::rtuple<EntityType>::type
			>::value
		> state_type;

	public:
		state_type change_state;

	public:
		bitset_change_tracker() {
		}

		bitset_change_tracker(const bool all_loaded) {
			change_state.set();
		}

		void update(const EntityType& entity) {
			//
		}

		const bool dirty() const {
			return true;
		}
	};

}}
