#pragma once
// TODO: Maybe extract exact includes?
#include "pk.hxx"

namespace moneta { namespace traits {

	namespace detail {
		namespace mpl {
			template <class EntityType>
			struct pk_tie : boost::mpl::transform<
				typename pk<EntityType>::type,
				boost::add_reference<boost::mpl::_>
			> {};

			template <class EntityType>
			struct const_pk_tie : boost::mpl::transform<
				typename pk<EntityType>::type,
				boost::add_reference<boost::add_const<boost::mpl::_> >
			> {};
		}

		namespace fusion {
			template <class EntityType>
			struct pk_tie : boost::fusion::result_of::as_vector<
				typename mpl::pk_tie<EntityType>::type
			> {};

			template <class EntityType>
			struct const_pk_tie : boost::fusion::result_of::as_vector<
				typename mpl::const_pk_tie<EntityType>::type
			> {};
		}

		template <class EntityType>
		struct entity_pk_tie : detail::deref_if_unary<
			typename detail::fusion::pk_tie<EntityType>::type
		> {};

		template <class EntityType>
		struct const_entity_pk_tie : detail::deref_if_unary<
			typename detail::fusion::const_pk_tie<EntityType>::type
		> {};

	}



	template <class EntityType, class Enable = void>
	struct pk_tie;

	template <class NonEntityType>
	struct pk_tie<
		NonEntityType,
		typename boost::enable_if<
			typename boost::mpl::not_<moneta::traits::is_entity<NonEntityType> >::type
		>::type
	> {
		typedef NonEntityType& type;

		type operator()(NonEntityType& value) {
			return value;
		}
	};

	template <class EntityType>
	struct pk_tie<
		EntityType,
		typename boost::enable_if<moneta::traits::is_entity<EntityType> >::type
	> {
		typedef typename moneta::traits::detail::entity_pk_tie<EntityType>::type type;
		
		type operator()(EntityType& entity) {
			// TODO: Implement
		}
	};



	template <class EntityType, class Enable = void>
	struct const_pk_tie;

	template <class NonEntityType>
	struct const_pk_tie<
		NonEntityType,
		typename boost::enable_if<
			typename boost::mpl::not_<moneta::traits::is_entity<NonEntityType> >::type
		>::type
	> {
		typedef typename boost::add_const<NonEntityType>::type& type;

		type operator()(type value) {
			return value;
		}
	};

	template <class EntityType>
	struct const_pk_tie<
		EntityType,
		typename boost::enable_if<moneta::traits::is_entity<EntityType> >::type
	> {
		typedef typename moneta::traits::detail::const_entity_pk_tie<EntityType>::type type;
		
		type operator()(const EntityType& entity) {
			// TODO: Implement
		}
	};

}}
