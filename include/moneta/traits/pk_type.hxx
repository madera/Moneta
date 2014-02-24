#pragma once
#include "members.hxx"
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/find_if.hpp>

#define MONETA_PRIMARY_KEY(member) \
	template<> struct moneta::traits::detail::is_pk<member> : boost::true_type {};

namespace moneta { namespace traits {

	namespace detail {
		template <class MemberType>
		struct is_pk : boost::false_type {
		};
	}

	template <class EntityType>
	struct has_pk : boost::mpl::not_<
		boost::is_same<
			typename boost::mpl::find_if<
				typename members<EntityType>::type,
				detail::is_pk<boost::mpl::_1>
			>::type,
			typename boost::mpl::end<
				typename members<EntityType>::type
			>::type
		>
	> {};

	//namespace detail {
	//	struct get_primary_key_memptr_types {
	//		template <class EntityType>
	//		struct apply : boost::mpl::copy_if<
	//			typename members_of<EntityType>::type,
	//			traits::is_pk<boost::mpl::_1>,
	//			boost::mpl::back_inserter<boost::mpl::vector<> >
	//		> {};
	//	};

	//	struct get_primary_key {
	//		template <class EntityType>
	//		struct apply : boost::mpl::transform<
	//			typename get_primary_key_memptr_types::apply<
	//				typename pure_type<EntityType>::type
	//			>::type,
	//			detail::get_result_type
	//		> {};
	//	};
	//}

	//template <class EntityType>
	//struct mpl_pk : boost::mpl::apply<detail::get_primary_key, EntityType> {};
	//
	//template <class EntityType>
	//struct fusion_pk : boost::fusion::result_of::as_vector<
	//	typename mpl_pk<EntityType>::type
	//> {};

	//template <class EntityType>
	//struct pk : boost::mpl::if_<
	//	boost::mpl::equal_to<
	//		typename boost::mpl::size<
	//			typename fusion_pk<EntityType>::type
	//		>::type,
	//		boost::mpl::int_<1>
	//	>,
	//	typename boost::mpl::at_c<typename fusion_pk<EntityType>::type, 0>::type,
	//	typename fusion_pk<EntityType>::type
	//> {};



	//template <class EntityType>
	//struct mpl_pk_tie : boost::mpl::transform<
	//	typename mpl_pk<EntityType>::type,
	//	boost::add_reference<boost::mpl::_>
	//> {};
	//
	//template <class EntityType>
	//struct fusion_pk_tie : boost::fusion::result_of::as_vector<
	//	typename mpl_pk_tie<EntityType>::type
	//> {};

	//template <class EntityType>
	//struct pk_tie : boost::mpl::if_<
	//	boost::mpl::equal_to<
	//		typename boost::mpl::size<
	//			typename fusion_pk_tie<EntityType>::type
	//		>::type,
	//		boost::mpl::int_<1>
	//	>,
	//	typename boost::mpl::at_c<typename fusion_pk_tie<EntityType>::type, 0>::type,
	//	typename fusion_pk_tie<EntityType>::type
	//> {};

	//namespace detail {

	//	template <class EntityType>
	//	typename pk_tie<EntityType>::type
	//	pk_extractor(EntityType& entity) {
	//		return fucker::detail::util::memptr_constructor<
	//			pk_tie<EntityType>::type,
	//			typename boost::mpl::apply<get_primary_key_memptr_types, EntityType>::type,
	//			EntityType,
	//			boost::mpl::size<typename pk_tie<EntityType>::type>::value
	//		>()(entity);
	//	}

	//} // namespace detail

	//template <class EntityType>
	//typename boost::enable_if<
	//	traits::is_fusion_vector<typename pk_tie<EntityType>::type>,
	//	typename pk_tie<EntityType>::type
	//>::type
	//extract_pk(EntityType& entity) {
	//	BOOST_STATIC_ASSERT(fucker::traits::has_pk<EntityType>::value);
	//	return detail::pk_extractor<EntityType>(entity);
	//}

	//template <class EntityType>
	//typename boost::enable_if<
	//	boost::mpl::not_<
	//		traits::is_fusion_vector<typename pk_tie<EntityType>::type>
	//	>,
	//	typename pk_tie<EntityType>::type
	//>::type
	//extract_pk(EntityType& entity) {
	//	BOOST_STATIC_ASSERT(fucker::traits::has_pk<EntityType>::value);
	//	
	//	typename boost::mpl::at_c<
	//		typename boost::mpl::apply<detail::get_primary_key_memptr_types, EntityType>::type, 0
	//	>::type single_memptr;

	//	return single_memptr(entity);
	//}

	//template <class EntityType>
	//typename const bool same_pk(const EntityType& a, const EntityType& b) {
	//	// FIXME: We don't have a const version of these dudes...
	//	EntityType copy_a = a;
	//	EntityType copy_b = b;
	//	return extract_pk(copy_a) == extract_pk(copy_b);
	//}

	//template <class EntityType>
	//std::vector<std::string> get_pk_type_names() {
	//	return fucker::get_member_names_from_vector<
	//		typename boost::mpl::apply<fucker::detail::get_primary_key_memptr_types, EntityType>::type
	//	>();
	//}








	//template <class EntityType>
	//typename boost::enable_if<
	//	traits::is_fusion_vector<typename pk_tie<EntityType>::type>,
	//	typename pk<EntityType>::type
	//>::type
	//empty_pk() {
	//	typename fucker::pk_type<EntityType>::type result;
	//	return boost::fusion::transform(result, fucker::detail::blanker());
	//}

	//template <class EntityType>
	//typename boost::enable_if<
	//	boost::mpl::not_<
	//		traits::is_fusion_vector<typename pk_tie<EntityType>::type>
	//	>,
	//	typename pk<EntityType>::type
	//>::type
	//empty_pk() {
	//	typename traits::pk<EntityType>::type result{};
	//	return result;
	//}




	//template <class EntityType>
	//const bool has_empty_pk(EntityType& entity) { // FIXME: Use const. This is a query function.
	//	return traits::extract_pk<EntityType>(entity) == empty_pk<EntityType>();
	//}

}}
