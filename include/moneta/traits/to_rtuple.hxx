#pragma once
#include "rtuple.hxx"
#include "../traits/detail/sepacon_opfx.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/tuple.hxx"
#include "../traits/pk.hxx"
#include "../traits/extract_pk.hxx"
#include "../traits/detail/blanker.hxx"
#include <boost/utility/enable_if.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityType>
		struct rtuple_maker {
			typename rtuple<EntityType>::type
			operator()(EntityType& entity) {
				return moneta::traits::detail::sepacon_opfx<
					moneta::traits::members<EntityType>::type,
					typename rtuple<EntityType>::type,
					EntityType&,
					moneta::traits::get_pk_functor
				>()(entity);
			}
		};

	}

	template <class EntityType>
	typename rtuple<EntityType>::type
	to_rtuple(EntityType& x) {
		return detail::rtuple_maker<EntityType>()(x);
	}

	// XXX: Move this somewhere else.
	template <class EntityType>
	typename traits::rtuple<EntityType>::type make_rtuple() {
		traits::rtuple<EntityType>::type result;
		return boost::fusion::transform(result, moneta::traits::detail::blanker());
	}

}}
