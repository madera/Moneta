// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "pure_type.hxx"
#include <boost/type_traits/integral_constant.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/mpl.hpp>
#include "../member.hxx"
#include <string>
#include <vector>

#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/size.hpp>

//
// Types of Traits:
//
//	* Entity Traits: Entity class traits
//	  Examples: entity_name and sql_table_name.
//
//	* Member Traits: Entity member traits
//	  Examples: member_name and sql_field_name.
//

#define MONETA_DECLARE_TRAIT(trait)                \
	namespace moneta { namespace traits {      \
		template <class T>                 \
		struct trait : boost::false_type { \
			typedef void trait_type;   \
		};                                 \
	}}

// TODO: Is this naming ok? Or should it be RT_TRAIT?
#define MONETA_DECLARE_TYPED_TRAIT(trait, type)    \
	namespace moneta { namespace traits {      \
		template <class T>                 \
		struct trait : boost::false_type { \
			typedef type trait_type;   \
		};                                 \
	}}

#define MONETA_DEFINE_TRAIT(trait, type, result_type)           \
	template <>                                             \
	struct moneta::traits::trait<type> : boost::true_type { \
		typedef result_type trait_type;                 \
	};

#define MONETA_DEFINE_TRAIT_WITH_GET(trait, type, return_type, value) \
	namespace moneta { namespace traits {           \
		template <>                             \
		struct trait<type> : boost::true_type { \
			typedef return_type trait_type; \
			static trait_type get() {       \
				return value;           \
			}                               \
		};                                      \
	}}

#define MONETA_DEFINE_FLAG_TRAIT(trait, type)                             \
	template <>                                                       \
	struct moneta::traits::trait<type> : boost::true_type {};

#define MONETA_DECLARE_ENTITY_TRAIT(trait) \
	MONETA_DECLARE_TRAIT(trait)

#define MONETA_DECLARE_MEMBER_TRAIT(trait) \
	MONETA_DECLARE_TYPED_TRAIT(trait, std::string)

// TODO: Add a nice assert to capture non-registered entities early.
// TODO: Deprecated.
#define MONETA_DEFINE_ENTITY_TRAIT_GETTER(trait, name)                   \
	template <class Entity>                                          \
	const typename moneta::traits::trait<                            \
		typename moneta::traits::pure_type<Entity>::type         \
	>::trait_type                                                    \
	name() {                                                         \
		return moneta::traits::trait<                            \
			typename moneta::traits::pure_type<Entity>::type \
		>::get();                                                \
	}

// XXX: Remove const on return value.
// XXX: Does this need 'Entity'?
#define MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(trait, type, name, members) \
	template <class Entity>                                                   \
	const std::vector<type> name() {                                          \
		return moneta::traits::get_member_traits_with_get<                \
			moneta::traits::trait,                                    \
			members                                                   \
		>();                                                              \
	}

#define MONETA_DEFINE_MEMBER_TRAIT_COLLECTOR(trait, trait_type, name) \
	MONETA_DEFINE_MEMBER_SEQUENCE_TRAIT_COLLECTOR(trait, trait_type, name, typename moneta::traits::members<Entity>::type)

namespace moneta { namespace traits {

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

		template <class Member>
		void operator()(Member) const {
			_target.push_back(MemberTraitWithGet<Member>::get());
		}
	};

	template <
		template <typename T>
		class MemberTraitWithGet,
		class ContainerType
	>
	trait_back_inserter_iterator<MemberTraitWithGet, ContainerType>
	trait_back_inserter(ContainerType& target) {
		return trait_back_inserter_iterator<MemberTraitWithGet, ContainerType>(target);
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
			MemberPointersSequence(),
			trait_back_inserter<MemberTraitWithGet>(result)
		);

		return result;
	}

}}
