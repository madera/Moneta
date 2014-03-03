// FIXME: This file needs work...
#pragma once
#include "is_entity.hxx"
#include "pk.hxx"
#include <boost/mpl/transform.hpp>
#include <boost/mpl/replace_if.hpp>
#include <boost/mpl/if.hpp>

namespace moneta { namespace traits {

	namespace detail {



		template <typename F>
		struct replace_entity_with_f {

			template <typename T>
			struct apply : boost::mpl::if_<
				is_entity<T>,
				typename boost::mpl::apply<F, T>::type,
				//int,
				T
			> {};

		};







		struct replace_entity_with_pk {
			template <typename T>
			struct apply : boost::mpl::if_<
				is_entity<T>,
				typename pk<T>::type,
				T
			> {};
		};

		struct replace_entity_with_pk_tie {
			template <typename T>
			struct apply : boost::mpl::if_<
				is_entity<T>,
				typename pk_tie<T>::type,
				T&
			> {};
		};

	}


	template <typename VectorType>
	struct replace_entities_with_pks : boost::mpl::transform<
		VectorType,
		detail::replace_entity_with_pk
	> {};

	template <typename VectorType>
	struct replace_entities_with_pk_ties : boost::mpl::transform<
		VectorType,
		detail::replace_entity_with_pk_tie
	> {};

}}
