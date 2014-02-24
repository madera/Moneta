#pragma once
#include "members_of.hxx"
#include "detail/sequence_parameter_constructor_sg.hxx"
#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/transform.hpp>

namespace moneta { namespace meta {

	namespace detail {
		struct get_pointer_type {
			template <typename T>
			struct apply {
				typedef typename T::result_type T::class_type::* type;
			};
		};
	}

	template <class EntityType>
	struct member_pointers {
		typedef typename boost::fusion::result_of::as_vector<
			typename boost::mpl::transform<
				typename members_of<EntityType>::type,
				detail::get_pointer_type
			>::type
		>::type type;

		static const type get() {
			return detail::sequence_parameter_constructor_sg<
				typename members_of<EntityType>::type,
				type
			>()();
		}
	};
}}
