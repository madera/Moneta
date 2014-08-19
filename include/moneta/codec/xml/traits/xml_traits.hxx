#pragma once
#include "../../../traits/detail/member_trait_base.hxx"
#include <boost/mpl/find_if.hpp>

MONETA_DECLARE_TRAIT(xml_attribute)

#define MONETA_XML_ATTIBUTE(klass, field) \
	MONETA_DEFINE_FLAG_TRAIT(xml_attribute, field)

namespace moneta { namespace codec { namespace detail {

	template <class Entity>
	struct has_xml_attribute : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename traits::members<Entity>::type,
				traits::detail::xml_attribute<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename traits::members<Entity>::type
			>::type
		>
	> {};

	template <class Member>
	struct is_first_xml_attribute : boost::mpl::if_<
		traits::detail::xml_attribute<typename Member>,
		boost::true_type,
		boost::false_type
	> {};

	template <class Entity>
	struct is_last_xml_attribute {
	};

	template <class Entity>
	struct is_first_xml_element {
	};

	template <class Entity>
	struct is_last_xml_element {
	};

}}}
