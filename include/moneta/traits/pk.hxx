#pragma once
#include "members.hxx"
#include "tuple.hxx" // for detail::get_result_type // TODO: Make this better.
#include "is_entity.hxx"
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>

#define MONETA_DECLARE_PRIMARY_KEY(k, t, m) \
	template<> struct moneta::traits::detail::is_pk< \
		moneta::traits::member<k, t, &k::m> \
	> : boost::true_type {};

namespace moneta { namespace traits {

	namespace detail {
		// XXX: Maybe export this? Looks useful...
		template <class MemberType>
		struct is_pk : boost::false_type {
		};
	}

	template <class EntityType>
	struct has_pk : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename members<EntityType>::type,
				detail::is_pk<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename members<EntityType>::type
			>::type
		>
	> {};

	namespace detail {
		struct get_pk_memptr_types {
			template <class EntityType>
			struct apply : boost::mpl::copy_if<
				typename members<EntityType>::type,
				is_pk<boost::mpl::_1>,
				boost::mpl::back_inserter<boost::mpl::vector<> >
			> {};
		};

		template <class EntityType>
		struct get_primary_key {
			template <class EntityType>
			struct apply : boost::mpl::transform<
				typename get_pk_memptr_types::apply<EntityType>::type,
				detail::get_result_type<EntityType>
			> {};
		};

		namespace mpl {
			template <class EntityType>
			struct pk : boost::mpl::apply<
				detail::get_primary_key<EntityType>,
				EntityType
			> {};
		}

		namespace fusion {
			template <class EntityType>
			struct pk : boost::fusion::result_of::as_vector<
				typename mpl::pk<EntityType>::type
			> {};
		}

		// XXX: Move.
		// Type: vector1<T> --> T, else: Sequence.
		template <class Sequence>
		struct deref_if_unary : boost::mpl::if_<
			boost::mpl::equal_to<
				typename boost::mpl::size<Sequence>::type,
				boost::mpl::int_<1>
			>,
			typename boost::mpl::at_c<Sequence, 0>::type,
			Sequence
		> {};

		template <class EntityType>
		struct entity_pk : deref_if_unary<
			typename detail::fusion::pk<EntityType>::type
		> {};
	}

	template <class EntityType>
	struct pk_members : boost::mpl::apply<
		detail::get_pk_memptr_types,
		EntityType
	> {};

	template <class EntityType, class Enable = void>
	struct pk;

	template <class NonEntityType>
	struct pk<
		NonEntityType,
		typename boost::enable_if<
			typename boost::mpl::not_<
				moneta::traits::is_entity<NonEntityType>
			>::type
		>::type
	> {
		typedef NonEntityType type;

		type operator()(NonEntityType& value) {
			return value;
		}
	};

	template <class EntityType>
	struct pk<
		EntityType,
		typename boost::enable_if<
			moneta::traits::is_entity<EntityType>
		>::type
	> {
		typedef typename moneta::traits::detail::entity_pk<EntityType>::type type;
		
		type operator()(EntityType& entity) {
			return moneta::traits::extract_pk(entity);
		}
	};

	template <typename T>
	struct get_pk_functor {
		typename pk<T>::type operator()(T& x) {
			return pk<T>()(x);
		}

		typename pk<const T>::type operator()(const T& x) {
			return pk<const T>()(x);
		}
	};

}}
