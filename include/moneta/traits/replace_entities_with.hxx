#pragma once
#include "is_entity.hxx"
#include "pk.hxx"
#include <boost/mpl/transform.hpp>
#include <boost/mpl/if.hpp>

namespace moneta { namespace traits {

	namespace detail {
		template <typename NewType>
		struct if_is_entity_substitute_with {
			template <typename T>
			struct apply : boost::mpl::if_<
				is_entity<T>,
				NewType,
				T
			> {};
		};

		struct if_is_entity_substitute_with_their_pk {
			template <typename T>
			struct apply : boost::mpl::if_<
				is_entity<T>,
				typename pk<T>::type,
				T
			> {};
		};
	}

	template <typename VectorType, typename NewType>
	struct replace_entities_with : boost::mpl::transform<
		VectorType,
		detail::if_is_entity_substitute_with<NewType>
	> {};

	template <typename VectorType>
	struct replace_entities_with_their_pk : boost::mpl::transform<
		VectorType,
		detail::if_is_entity_substitute_with_their_pk
	> {};
}}
