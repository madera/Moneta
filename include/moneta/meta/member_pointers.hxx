#pragma once
#include "members_of.hxx"
#include "detail/sequence_parameter_constructor_sg.hxx"
// TODO: Filter out exact include for fusion::result_of.
#include <boost/fusion/algorithm.hpp> // for boost::fusion::result_of::as_vector
#include <boost/mpl/transform.hpp>

//#pragma once
//#include "detail/member_trait_base.hxx"
//#include <boost/fusion/algorithm/iteration/for_each.hpp>
//#include <boost/fusion/mpl.hpp>
//
//MONETA_DECLARE_MEMBER_TRAIT(member_name, std::string)
//
//#define MONETA_MEMBER_POINTER(member, name) \
//	MONETA_DEFINE_MEMBER_TRAIT(member_name, member, std::string, name)
//
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
