#pragma once
#include "pk.hxx"
#include "is_entity.hxx"

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityType, class Enable = void>
		struct pk_tie_or_type;

		template <class NonEntityType>
		struct pk_tie_or_type<
			NonEntityType,
			typename boost::enable_if<
				typename boost::mpl::not_<moneta::traits::is_entity<NonEntityType> >::type
			>::type
		> {
			typedef NonEntityType& type;
		};

		template <class EntityType>
		struct pk_tie_or_type<
			EntityType,
			typename boost::enable_if<moneta::traits::is_entity<EntityType> >::type
		> {
			typedef typename moneta::traits::pk_tie<EntityType>::type type;
		};

	}

	namespace detail {

		template <class EntityType, class Enable = void>
		struct const_pk_tie_or_type;

		template <class NonEntityType>
		struct const_pk_tie_or_type<
			NonEntityType,
			typename boost::enable_if<
				typename boost::mpl::not_<moneta::traits::is_entity<NonEntityType> >::type
			>::type
		> {
			typedef typename boost::add_const<NonEntityType>::type& type;
		};

		template <class EntityType>
		struct const_pk_tie_or_type<
			EntityType,
			typename boost::enable_if<moneta::traits::is_entity<EntityType> >::type
		> {
			typedef typename moneta::traits::const_pk_tie<EntityType>::type type;
		};

	}

}}
