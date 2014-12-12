#pragma once
// TODO: Maybe extract exact includes?
#include "pk.hxx"
#include "detail/sepacon_opfx.hxx"

namespace moneta { namespace traits {

	namespace detail {
		namespace mpl {
			template <class Entity>
			struct pk_tie : boost::mpl::transform<
				typename pk<Entity>::type,
				boost::add_reference<boost::mpl::_>
			> {};
		}

		namespace fusion {
			template <class Entity>
			struct pk_tie : boost::fusion::result_of::as_vector<
				typename mpl::pk_tie<Entity>::type
			> {};
		}

		template <class Entity>
		struct entity_pk_tie : detail::deref_if_unary<
			typename detail::fusion::pk_tie<Entity>::type
		> {};

	}

	template <class Entity, class Enable = void>
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

	template <class Entity>
	struct pk_tie<
		Entity,
		typename boost::enable_if<moneta::traits::is_entity<Entity> >::type
	> {
		typedef typename moneta::traits::detail::entity_pk_tie<Entity>::type type;
		
		typename type operator()(Entity& entity) {
			return moneta::traits::detail::sepacon_opfx<
				moneta::traits::pk_members<Entity>::type,
				type,
				Entity&
			>()(entity);
		}
	};

	template <typename T>
	struct get_pk_tie_functor {
		typename pk_tie<T>::type operator()(T& x) {
			return pk_tie<T>()(x);
		}

		typename pk_tie<const T>::type operator()(const T& x) {
			return pk_tie<const T>()(x);
		}
	};

}}
