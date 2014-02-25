#pragma once
#include "traits/tuple.hxx"
#include "traits/to_tie.hxx"
#include "traits/detail/blanker.hxx"
#include <boost/fusion/algorithm/transformation/transform.hpp>

namespace moneta {

	template <class EntityType>
	typename traits::tuple<EntityType>::type make_tuple() {
		traits::tuple<EntityType>::type result;
		return boost::fusion::transform(result, traits::detail::blanker());
	}

	template <class EntityType>
	EntityType make_entity() {
		EntityType result;
		traits::to_tie<EntityType>(result) = make_tuple<EntityType>();
		return result;
	}

}
