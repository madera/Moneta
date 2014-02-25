#pragma once
#include "members.hxx"
#include "tuple.hxx" // for detail::get_result_type // TODO: Make this better.
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>

#define MONETA_PRIMARY_KEY(member) \
	template<> struct moneta::traits::detail::is_pk<member> : boost::true_type {};

namespace moneta { namespace traits {

	namespace detail {
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
				traits::detail::is_pk<boost::mpl::_1>,
				boost::mpl::back_inserter<boost::mpl::vector<> >
			> {};
		};

		struct get_primary_key {
			template <class EntityType>
			struct apply : boost::mpl::transform<
				typename get_pk_memptr_types::apply<EntityType>::type,
				detail::get_result_type
			> {};
		};

		namespace mpl {
			template <class EntityType>
			struct pk : boost::mpl::apply<
				detail::get_primary_key,
				EntityType
			> {};
		}

		namespace fusion {
			template <class EntityType>
			struct pk : boost::fusion::result_of::as_vector<
				typename mpl::pk<EntityType>::type
			> {};
		}
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
	struct pk : deref_if_unary<
		typename detail::fusion::pk<EntityType>::type
	> {};

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
			> {}; // XXX: Needs test case.
		}

		namespace fusion {
			template <class EntityType>
			struct pk_tie : boost::fusion::result_of::as_vector<
				typename mpl::pk_tie<EntityType>::type
			> {};

			template <class EntityType>
			struct const_pk_tie : boost::fusion::result_of::as_vector<
				typename mpl::const_pk_tie<EntityType>::type
			> {}; // XXX: Needs test case.
		}
	}

	template <class EntityType>
	struct pk_tie : deref_if_unary<
		typename detail::fusion::pk_tie<EntityType>::type
	> {};

	template <class EntityType>
	struct const_pk_tie : deref_if_unary<
		typename detail::fusion::const_pk_tie<EntityType>::type
	> {}; // XXX: Needs test case.
}}
