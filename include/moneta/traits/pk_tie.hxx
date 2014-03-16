#pragma once
// TODO: Maybe extract exact includes?
#include "pk.hxx"
#include "detail/sepacon_opfx.hxx"

namespace moneta { namespace traits {

	namespace detail {
		namespace mpl {
			template <class EntityType>
			struct pk_tie : boost::mpl::transform<
				typename pk<EntityType>::type,
				boost::add_reference<boost::mpl::_>
			> {};
		}

		namespace fusion {
			template <class EntityType>
			struct pk_tie : boost::fusion::result_of::as_vector<
				typename mpl::pk_tie<EntityType>::type
			> {};
		}

		template <class EntityType>
		struct entity_pk_tie : detail::deref_if_unary<
			typename detail::fusion::pk_tie<EntityType>::type
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
		
		typename type operator()(EntityType& entity) {
			return moneta::traits::detail::sepacon_opfx<
				moneta::traits::pk_members<EntityType>::type,
				type,
				EntityType&
			>()(entity);
		}
	};

	template <typename T>
	struct get_pk_tie_functor {
		typename pk_tie<T>::type operator()(T& x) {
			return pk_tie<T>()(x);
		}
	};

}}
