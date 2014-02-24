#pragma once
#include "members_of.hxx"
#include <boost/fusion/include/as_vector.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/transform.hpp>

namespace moneta { namespace meta {

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
				typename members_of<EntityType>::type,
				get_result_type
			> {};
		}
	}

	template <typename EntityType>
	struct tuple : boost::fusion::result_of::as_vector<
		typename detail::mpl::vector<EntityType>::type
	> {};

	template <typename EntityType>
	struct tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename detail::mpl::vector<EntityType>::type,
			boost::add_reference<boost::mpl::_1>
		>::type
	> {};

	template <typename EntityType>
	struct const_tie : boost::fusion::result_of::as_vector<
		typename boost::mpl::transform<
			typename detail::mpl::vector<EntityType>::type,
			boost::add_reference<boost::add_const<boost::mpl::_1> >
		>::type
	> {};

}}
