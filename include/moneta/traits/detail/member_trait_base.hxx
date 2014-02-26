#pragma once
#include <boost/type_traits/integral_constant.hpp> // boost::false_type

// XXX: Re-evaluate the need for boost::false_type
#define MONETA_DECLARE_MEMBER_TRAIT(name, type) \
	namespace moneta { namespace traits { namespace detail { \
		template <class EntityType> \
		struct name : boost::false_type { \
			typedef type trait_type; \
		}; \
	}}}

#define MONETA_DEFINE_MEMBER_TRAIT(trait_class, member, type, value) \
	template <> \
	struct moneta::traits::detail::trait_class<member> { \
		typedef type trait_type; \
		\
		static trait_type get() { \
			return #value; \
		} \
	};

#define MONETA_DEFINE_MEMBER_TRAIT_ACCESSOR(trait_class, name) \
	template <class EntityType> \
	std::vector<std::string> name() { \
		return moneta::traits::detail::get_member_traits< \
			moneta::traits::detail::trait_class, \
			moneta::traits::members<EntityType>::type \
		>(); \
	}

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
	std::vector<std::string> get_member_traits() {
		std::vector<std::string> result;
		boost::fusion::for_each(
			typename MemberPointersSequence(),
			detail::trait_back_inserter<MemberTraitWithGet>(result)
		);

		return result;
	}

}}}
