#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../sql/traits/db_tuple.hxx"
#include "../sql/traits/to_db_tuple.hxx"
#include "../sql/traits/to_db_tie.hxx"
#include <bitset>

namespace moneta { namespace container {

	template <class EntityType>
	struct null_change_tracker {
		null_change_tracker() {
		}

		null_change_tracker(const EntityType& ignored) {
		}

		void update(const EntityType& entity) {
		}

		const bool dirty() const {
			return true;
		}
	};

}}
