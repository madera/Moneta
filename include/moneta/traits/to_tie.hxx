#pragma once
#include "tie.hxx"
#include "detail/sepacon_opfx.hxx"

namespace moneta { namespace traits {

	namespace detail {
		template <class Entity>
		struct tie_maker {
			typename tie<Entity>::type
			operator()(Entity& entity) {
				return sepacon_opfx<
					typename members<Entity>::type,
					typename tie<Entity>::type,
					Entity&
				>()(entity);
			}
		};
	}

	template <class Entity>
	typename tie<Entity>::type
	to_tie(Entity& x) {
		return detail::tie_maker<Entity>()(x);
	}

}}
