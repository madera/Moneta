#pragma once
#include "../../../traits/members.hxx"
#include "../../../traits/detail/member_trait_base.hxx"
#include <boost/mpl/find_if.hpp>

MONETA_DECLARE_TRAIT(xml_attribute)
#define MONETA_XML_ATTIBUTE(klass, field) \
	MONETA_DEFINE_FLAG_TRAIT(xml_attribute, field)

MONETA_DECLARE_MEMBER_TRAIT(xml_container_member_name)
#define MONETA_XML_CONTAINER_MEMBER_NAME(member, name) \
	MONETA_DEFINE_TRAIT_WITH_GET(xml_container_member_name, member, std::string, BOOST_PP_STRINGIZE(name))

MONETA_DECLARE_MEMBER_TRAIT(xml_container_member_element_name)
#define MONETA_XML_CONTAINER_MEMBER_ELEMENT_NAME(member, name) \
	MONETA_DEFINE_TRAIT_WITH_GET(xml_container_member_element_name, member, std::string, BOOST_PP_STRINGIZE(name))

namespace moneta { namespace codec { namespace detail {

	template <class Member>
	struct is_xml_attribute : traits::detail::xml_attribute<Member> {};

	template <class Member>
	struct is_xml_element : boost::mpl::not_<traits::detail::xml_attribute<Member> > {};

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
