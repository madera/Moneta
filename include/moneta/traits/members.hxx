#pragma once
#include "detail/member_trait_base.hxx"
#include <boost/mpl/copy_if.hpp>

MONETA_DECLARE_ENTITY_TRAIT(members_of)

namespace moneta { namespace traits {

	template <class Entity>
	struct members : members_of<
		typename pure_type<Entity>::type
	> {};

	template <class Member>
	struct is_first_member : boost::is_same<
		typename boost::mpl::front<
			typename members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	template <class Member>
	struct is_last_member : boost::is_same<
		typename boost::mpl::back<
			typename members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	//

	template <class Member>
	struct is_member_entity : boost::mpl::not_<
		boost::is_same<
			// Basically, is_entity:
			typename members<
				typename Member::result_type
			>::type,
			boost::false_type
		>
	> {};

	//

	template <class Entity>
	struct non_entity_members : boost::mpl::copy_if<
		typename members<Entity>::type,
		boost::mpl::not_<
			is_member_entity<boost::mpl::_1>
		>,
		boost::mpl::back_inserter<boost::mpl::vector0<> >
	> {};

	template <class Member>
	struct is_first_non_entity_member : boost::is_same<
		typename boost::mpl::front<
			typename non_entity_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	template <class Member>
	struct is_last_non_entity_member : boost::is_same<
		typename boost::mpl::back<
			typename non_entity_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	//

	template <class Entity>
	struct entity_members : boost::mpl::copy_if<
		typename members<Entity>::type,
		is_member_entity<boost::mpl::_1>,
		boost::mpl::back_inserter<boost::mpl::vector0<> >
	> {};

	template <class Member>
	struct is_first_entity_member : boost::is_same<
		typename boost::mpl::front<
			typename entity_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

	template <class Member>
	struct is_last_entity_member : boost::is_same<
		typename boost::mpl::back<
			typename entity_members<typename Member::class_type>::type
		>::type,
		Member
	> {};

}}
