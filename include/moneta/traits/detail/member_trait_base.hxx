#pragma once
#include "pure_type.hxx"
// XXX: Re-evaluate the need for boost::false_type
#include <boost/type_traits/integral_constant.hpp> // boost::false_type
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>

//
// Types of Traits:
//
//	* Entity Traits: Entity class traits
//	  Examples: entity_name and sql_table_name.
//
//	* Member Traits: Entity member traits
//	  Examples: member_name and sql_field_name.
//

#define MONETA_DECLARE_TRAIT(trait, type)                        \
	namespace moneta { namespace traits { namespace detail { \
		template <class EntityType>                      \
		struct trait : boost::false_type {               \
			typedef type trait_type;                 \
		};                                               \
	}}}

#define MONETA_DEFINE_TRAIT_WITH_GET(trait, type, get_type, get_value) \
	template <>                                                    \
	struct moneta::traits::detail::trait<type> {                   \
		typedef get_type trait_type;                           \
		static trait_type get() {                              \
			return get_value;                              \
		}                                                      \
	};



#define MONETA_DECLARE_ENTITY_TRAIT(trait, type) \
	MONETA_DECLARE_TRAIT(trait, type)

#define MONETA_DECLARE_MEMBER_TRAIT(trait, type) \
	MONETA_DECLARE_TRAIT(trait, type)



// TODO: Add a nice assert to capture non-registered entities early.
#define MONETA_DEFINE_ENTITY_TRAIT_GETTER(trait, name)               \
	template <class EntityType>                                  \
	const typename moneta::traits::detail::trait<                \
		typename moneta::traits::pure_type<EntityType>::type \
	>::trait_type                                                \
	name() {                                                     \
		return moneta::traits::detail::trait<                \
			moneta::traits::pure_type<EntityType>::type  \
		>::get();                                            \
	}

#define MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(trait, name, members) \
	template <class EntityType>                                         \
	const std::vector<moneta::traits::detail::trait::trait_type>        \
	name() {                                                            \
		return moneta::traits::detail::get_member_traits_with_get<  \
			moneta::traits::detail::trait,                      \
			members                                             \
		>();                                                        \
	}

#define MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(trait, name) \
	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(trait, name, moneta::traits::members<EntityType>::type)

namespace moneta { namespace traits { namespace detail {

	template <
		template <typename T>
		class MemberTraitWithGet,
		class ContainerType
	>
	struct trait_back_inserter_iterator  {
		ContainerType& _target;

		trait_back_inserter_iterator(ContainerType& target)
			: _target(target) {
		}

		template <class MemberType>
		void operator()(MemberType& member) const {
			_target.push_back(MemberTraitWithGet<MemberType>::get());
		}
	};

	template <
		template <typename T>
		class MemberTraitWithGet,
		class ContainerType
	>
	detail::trait_back_inserter_iterator<MemberTraitWithGet, ContainerType>
	trait_back_inserter(ContainerType& target) {
		return detail::trait_back_inserter_iterator<MemberTraitWithGet, ContainerType>(target);
	}

	template <
		template <typename T>
		class MemberTraitWithGet,
		class MemberPointersSequence
	>
	std::vector<std::string> get_member_traits_with_get() {
		std::vector<std::string> result;
		boost::fusion::for_each(
			typename MemberPointersSequence(),
			detail::trait_back_inserter<MemberTraitWithGet>(result)
		);

		return result;
	}

}}}
