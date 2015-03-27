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
#include "../../../traits/is_entity.hxx"
#include "../../../traits/members.hxx"
#include "../../../traits/entity_name.hxx"
#include "../../../traits/member_name.hxx"
#include "../../../traits/detail/member_trait_base.hxx"
#include "../../../algorithm/detail/path.hxx"
#include <boost/mpl/find_if.hpp>

// --------------------------------------------------------------------------------------------------------------------

MONETA_DECLARE_TRAIT(xml_attribute)

#define MONETA_XML_ATTIBUTE(klass, field)                          \
	namespace moneta { namespace traits {                      \
		template <>                                        \
		struct xml_attribute<field> : boost::true_type {}; \
	}}

// --------------------------------------------------------------------------------------------------------------------

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityOrMember, class Enable = void>
		struct default_xml_item_name {
			typedef std::string trait_type;

			static trait_type get() {
				return member_name<EntityOrMember>::get();
			}
		};

		template <class EntityOrMember>
		struct default_xml_item_name<
			EntityOrMember,
			typename boost::enable_if<
				is_entity<EntityOrMember>
			>::type
		> {
			typedef std::string trait_type;

			static trait_type get() {
				return entity_name<EntityOrMember>::get();
			}
		};

	}

	template <class Member>
	struct xml_item_name {
		typedef std::string trait_type;
		static trait_type get() {
			return detail::default_xml_item_name<Member>::get();
		}
	};

}}

#define MONETA_XML_ITEM_NAME(member, name)                        \
	namespace moneta { namespace traits {                     \
		template <>                                       \
		struct xml_item_name<member> : boost::true_type { \
			typedef std::string trait_type;           \
			static trait_type get() {                 \
				return BOOST_PP_STRINGIZE(name);  \
			}                                         \
		};                                                \
	}}

// ---

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityOrMember, class Path, class Enable = void>
		struct get_xml_item_name_impl {
			typedef std::string trait_type;

			static trait_type get() {
				return xml_item_name<EntityOrMember>::get();
			}
		};

		template <class EntityOrMember, class Path>
		struct get_xml_item_name_impl<
			EntityOrMember,
			Path,
			typename boost::enable_if<
				boost::mpl::and_<
					is_entity<EntityOrMember>,
					boost::mpl::empty<Path>
				>
			>::type
		> {
			typedef std::string trait_type;

			static trait_type get() {
				return xml_item_name<EntityOrMember>::get();
			}
		};

		template <class EntityOrMember, class Path>
		struct get_xml_item_name_impl<
			EntityOrMember,
			Path,
			typename boost::enable_if<
				boost::mpl::and_<
					is_entity<EntityOrMember>,
					boost::mpl::not_<
						boost::mpl::empty<Path>
					>,
					boost::mpl::not_<
						algorithm::detail::is_cwd_container_member<Path>
					>
				>
			>::type
		> {
			typedef std::string trait_type;

			static trait_type get() {
				return xml_item_name<
					typename boost::mpl::back<Path>::type
				>::get();
			}
		};

		template <class EntityOrMember, class Path>
		struct get_xml_item_name_impl<
			EntityOrMember,
			Path,
			typename boost::enable_if<
				boost::mpl::and_<
					is_entity<EntityOrMember>,
					boost::mpl::not_<
						boost::mpl::empty<Path>
					>,
					algorithm::detail::is_cwd_container_member<Path>
				>
			>::type
		> {
			typedef std::string trait_type;

			static trait_type get() {
				return xml_item_name<EntityOrMember>::get();
			}
		};

		
	}

	template <class Entity, class Path>
	std::string get_xml_item_name() {
		return detail::get_xml_item_name_impl<Entity, Path>::get();
	}

}}

// --------------------------------------------------------------------------------------------------------------------

namespace moneta { namespace traits {

	template <class Member>
	struct xml_container_item_name {
		typedef std::string trait_type;
		static trait_type get() {
			const std::string text = member_name<Member>::get();
			return (text[0] >= 'A' && text[0] <= 'Z')?
				text + "Item" :
				text + "_item";
		}
	};

}}

#define MONETA_XML_CONTAINER_ITEM_NAME(member, name)                        \
	namespace moneta { namespace traits {                               \
		template <>                                                 \
		struct xml_container_item_name<member> : boost::true_type { \
			typedef std::string trait_type;                     \
			static trait_type get() {                           \
				return BOOST_PP_STRINGIZE(name);            \
			}                                                   \
		};                                                          \
	}}

// --------------------------------------------------------------------------------------------------------------------

namespace moneta { namespace traits {

	template <class Member>
	struct is_xml_attribute : xml_attribute<
		Member
	> {};

	template <class Member>
	struct is_xml_element : boost::mpl::not_<
		xml_attribute<Member>
	> {};

// --------------------------------------------------------------------------------------------------------------------

	template <class Entity>
	struct has_xml_attributes : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename members<Entity>::type,
				is_xml_attribute<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename members<Entity>::type
			>::type
		>
	> {};

// --------------------------------------------------------------------------------------------------------------------

	template <class Entity>
	struct xml_attribute_members : boost::mpl::copy_if<
		typename members<Entity>::type,
		is_xml_attribute<boost::mpl::_1>,
		boost::mpl::back_inserter<boost::mpl::vector0<> >
	> {};

// --------------------------------------------------------------------------------------------------------------------

	template <class Member>
	struct is_first_xml_attribute_member : boost::is_same<
		typename boost::mpl::front<
			typename xml_attribute_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	template <class Member>
	struct is_last_xml_attribute_member : boost::is_same<
		typename boost::mpl::back<
			typename xml_attribute_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

// --------------------------------------------------------------------------------------------------------------------

	template <class Entity>
	struct has_xml_elements : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename members<Entity>::type,
				is_xml_element<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename members<Entity>::type
			>::type
		>
	> {};

// --------------------------------------------------------------------------------------------------------------------

	template <class Entity>
	struct xml_element_members : boost::mpl::copy_if<
		typename members<Entity>::type,
		is_xml_element<boost::mpl::_1>,
		boost::mpl::back_inserter<boost::mpl::vector0<> >
	> {};

// --------------------------------------------------------------------------------------------------------------------

	template <class Member>
	struct is_first_xml_element_member : boost::is_same<
		typename boost::mpl::front<
			typename xml_element_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	template <class Member>
	struct is_last_xml_element_member : boost::is_same<
		typename boost::mpl::back<
			typename xml_element_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

}}
