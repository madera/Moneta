#pragma once
#include "pure_type.hxx"
#include <boost/type_traits/integral_constant.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include "../member.hxx"
#include <string>
#include <vector>

//
// Types of Traits:
//
//	* Entity Traits: Entity class traits
//	  Examples: entity_name and sql_table_name.
//
//	* Member Traits: Entity member traits
//	  Examples: member_name and sql_field_name.
//

#define MONETA_DECLARE_TRAIT(trait)                              \
	namespace moneta { namespace traits { namespace detail { \
		template <class T>                               \
		struct trait : boost::false_type {               \
		};                                               \
	}}}

#define MONETA_DEFINE_TRAIT(trait, type, result_type)        \
	template <>                                                     \
	struct moneta::traits::detail::trait<type> : boost::true_type { \
		typedef result_type trait_type;                         \
	};

#define MONETA_DEFINE_TRAIT_WITH_GET(trait, type, get_type, get_value)  \
	template <>                                                     \
	struct moneta::traits::detail::trait<type> : boost::true_type { \
		typedef get_type trait_type;                            \
		static trait_type get() {                               \
			return get_value;                               \
		}                                                       \
	};

#define MONETA_DEFINE_FLAG_TRAIT(trait, type)                             \
	template <>                                                       \
	struct moneta::traits::detail::trait<type> : boost::true_type {};

#define MONETA_DECLARE_ENTITY_TRAIT(trait) \
	MONETA_DECLARE_TRAIT(trait)

#define MONETA_DECLARE_MEMBER_TRAIT(trait) \
	MONETA_DECLARE_TRAIT(trait)



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

// XXX: Remove const on return value.
// XXX: Does this need 'EntityType'?
#define MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(trait, type, name, members) \
	template <class EntityType>                                               \
	const std::vector<type> name() {                                          \
		return moneta::traits::detail::get_member_traits_with_get<        \
			moneta::traits::detail::trait,                            \
			members                                                   \
		>();                                                              \
	}

#define MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(trait, trait_type, name) \
	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(trait, trait_type, name, moneta::traits::members<EntityType>::type)

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

	// XXX: Type T is ignored...
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
