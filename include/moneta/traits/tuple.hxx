#pragma once
#include "members.hxx"
#include <boost/fusion/include/as_vector.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/transform.hpp>

// For our users...
// FIXME: Get this somewhere... but not here.
#include <boost/fusion/include/at_c.hpp>
#include <boost/mpl/at.hpp>

namespace moneta { namespace traits {

	namespace detail {

		struct get_result_type {
			template <typename T>
			struct apply {
				typedef typename T::result_type type;
			};
		};

		namespace mpl {

			template <typename EntityType>
			struct vector : boost::mpl::transform<
				typename members<EntityType>::type,
				get_result_type
			> {};

		}
	}

	template <typename EntityType>
	struct tuple : boost::fusion::result_of::as_vector<
		typename detail::mpl::vector<EntityType>::type
	> {};

}}
