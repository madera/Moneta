#pragma once

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <class EntityType>
		struct decoder;

	}

	template <class EntityType, class IteratorType>
	int decode(IteratorType begin, IteratorType end, EntityType& entity) {
		return detail::decoder<EntityType>()(begin, end, entity);
	}

}}}
