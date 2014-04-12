#pragma once
#include "rtuple.hxx"
#include "detail/indices_of.hxx"
#include "detail/sub_tie_vector.hxx"
#include "detail/deref_if_unary.hxx"
#include "detail/indices_of.hxx"

namespace moneta { namespace traits {

	template <class EntityType>
	struct to_pk_rtuple_impl {
		typedef typename moneta::traits::rtuple<EntityType>::type rtuple_type;

		typedef typename moneta::traits::detail::indices_of<
			typename moneta::traits::members<EntityType>::type,
			moneta::traits::detail::is_pk<boost::mpl::_1>
		>::type indices_type;

		typedef moneta::traits::detail::sub_tie_vector<
			rtuple_type,
			indices_type
		> sub_tie_type;

		typedef typename moneta::traits::detail::deref_if_unary<
			typename sub_tie_type::type
		>::type type;

		type operator()(rtuple_type& rtuple) const {
			return moneta::traits::detail::deref_unary_vector(
				sub_tie_type()(rtuple)
			);
		}

		type operator()(const rtuple_type& rtuple) const {
			return moneta::traits::detail::deref_unary_vector(
				sub_tie_type()(rtuple)
			);
		}
	};

	template <class EntityType>
	typename to_pk_rtuple_impl<EntityType>::type
	to_pk_rtuple(typename moneta::traits::rtuple<EntityType>::type& rtuple) {
		return to_pk_rtuple_impl<EntityType>()(rtuple);
	}

	template <class EntityType>
	typename to_pk_rtuple_impl<typename boost::add_const<EntityType>::type>::type
	to_pk_rtuple(
		const typename moneta::traits::rtuple<
			typename boost::add_const<EntityType>::type
		>::type& rtuple
	) {
		return to_pk_rtuple_impl<typename boost::add_const<EntityType>::type>()(rtuple);
	}
}}
