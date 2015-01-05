#pragma once
#include "extract_pk_fwd.hxx"
#include "tuple.hxx" // for detail::get_result_type // TODO: Make this better.
#include "is_entity.hxx"
#include "detail/deref_if_unary.hxx"
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
	namespace moneta { namespace traits { namespace detail { \
		template<> struct is_pk< \
			moneta::traits::member<k, t, &k::m> \
		> : boost::true_type {}; \
	}}}

namespace moneta { namespace traits {

	namespace detail {
		// XXX: Maybe export this? Looks useful...
		template <class MemberType>
		struct is_pk : boost::false_type {
		};
	}

	template <class Entity>
	struct has_pk : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename members<Entity>::type,
				detail::is_pk<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename members<Entity>::type
			>::type
		>
	> {};

	namespace detail {
		struct get_pk_memptr_types {
			template <class Entity>
			struct apply : boost::mpl::copy_if<
				typename members<Entity>::type,
				is_pk<boost::mpl::_1>,
				boost::mpl::back_inserter<boost::mpl::vector<> >
			> {};
		};

		struct get_primary_key {
			template <class Entity>
			struct apply : boost::mpl::transform<
				typename get_pk_memptr_types::apply<Entity>::type,
				detail::get_result_type<Entity>
			> {};
		};

		namespace mpl {
			template <class Entity>
			struct pk : boost::mpl::apply<
				detail::get_primary_key,
				Entity
			> {};
		}

		namespace fusion {
			template <class Entity>
			struct pk : boost::fusion::result_of::as_vector<
				typename mpl::pk<Entity>::type
			> {};
		}

		template <class Entity>
		struct entity_pk : detail::deref_if_unary<
			typename detail::fusion::pk<Entity>::type
		> {};
	}

	// TODO: Export this on it's own header.
	template <class Entity>
	struct pk_members : boost::mpl::apply<
		detail::get_pk_memptr_types,
		Entity
	> {};

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

	// TODO: Put this in a fwd?
//	template <class T, class U>
//	U extract_pk(T&);

	template <class Entity>
	struct pk<
		Entity,
		typename boost::enable_if<
			moneta::traits::is_entity<Entity>
		>::type
	> {
		typedef typename moneta::traits::detail::entity_pk<Entity>::type type;
		
		type operator()(Entity& entity) {
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
