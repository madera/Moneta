#pragma once
#include "../../../traits/members.hxx"
#include "../../../traits/member_name.hxx"
#include "../../../traits/detail/member_trait_base.hxx"
#include <boost/mpl/find_if.hpp>

MONETA_DECLARE_TRAIT(xml_attribute)

#define MONETA_XML_ATTIBUTE(klass, field) \
	namespace moneta { namespace traits { \
		template <> \
		struct xml_attribute<field> : boost::true_type {};\
	}}

// --------------------------------------------------------------------------------------------------------------------

namespace moneta { namespace traits { namespace detail {                             \
	template <class Member>                                                      \
	struct xml_container_item_name {                                             \
			typedef std::string trait_type;                              \
			static trait_type get() {                                    \
				const std::string text = member_name<Member>::get(); \
				return (text[0] >= 'A' && text[0] <= 'Z')?           \
					text + "Item" :                              \
					text + "_item";                              \
			}                                                            \
	};                                                                           \
}}}

#define MONETA_XML_CONTAINER_ITEM_NAME(member, name)                        \
	namespace moneta { namespace traits { namespace detail {            \
		template <>                                                 \
		struct xml_container_item_name<member> : boost::true_type { \
			typedef std::string trait_type;                     \
			static trait_type get() {                           \
				return BOOST_PP_STRINGIZE(name);            \
			}                                                   \
		};                                                          \
	}}}

// --------------------------------------------------------------------------------------------------------------------

namespace moneta { namespace codec { namespace detail {

	template <class Member>
	struct is_xml_attribute : traits::xml_attribute<
		Member
	> {};

	template <class Member>
	struct is_xml_element : boost::mpl::not_<
		traits::xml_attribute<Member>
	> {};

	//

	template <class Entity>
	struct has_xml_attributes : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename traits::members<Entity>::type,
				is_xml_attribute<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename traits::members<Entity>::type
			>::type
		>
	> {};

	template <class Entity>
	struct xml_attribute_members : boost::mpl::copy_if<
		typename traits::members<Entity>::type,
		is_xml_attribute<boost::mpl::_1>,
		boost::mpl::back_inserter<boost::mpl::vector0<> >
	> {};

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

	//

	template <class Entity>
	struct has_xml_elements : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename traits::members<Entity>::type,
				is_xml_element<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename traits::members<Entity>::type
			>::type
		>
	> {};

	template <class Entity>
	struct xml_element_members : boost::mpl::copy_if<
		typename traits::members<Entity>::type,
		is_xml_element<boost::mpl::_1>,
		boost::mpl::back_inserter<boost::mpl::vector0<> >
	> {};

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

}}}
