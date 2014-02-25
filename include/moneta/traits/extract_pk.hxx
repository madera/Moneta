#pragma once
#include "pk.hxx"
#include "detail/memptr_constructor.hxx"
#include "detail/is_fusion_vector.hxx"
//#include <boost/type_traits/integral_constant.hpp>
//#include <boost/type_traits/is_same.hpp>
//#include <boost/mpl/not.hpp>
//#include <boost/mpl/transform.hpp>
//#include <boost/mpl/copy_if.hpp>
//#include <boost/mpl/find_if.hpp>
//#include <boost/mpl/vector.hpp>
//#include <boost/mpl/if.hpp>
//#include <boost/mpl/size.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityType>
		typename pk_tie<EntityType>::type
		pk_extractor(EntityType& entity) {
			return moneta::traits::detail::memptr_constructor<
				typename pk_tie<EntityType>::type,
				typename boost::mpl::apply<get_pk_memptr_types, EntityType>::type,
				EntityType,
				boost::mpl::size<
					typename pk_tie<EntityType>::type
				>::value
			>()(entity);
		}

	}

	template <class EntityType>
	typename boost::enable_if<
		traits::detail::is_fusion_vector<typename pk_tie<EntityType>::type>,
		typename pk_tie<EntityType>::type
	>::type
	extract_pk(EntityType& entity) {
		BOOST_MPL_ASSERT((moneta::traits::has_pk<EntityType>));
		return detail::pk_extractor<EntityType>(entity);
	}


	template <class EntityType>
	typename boost::enable_if<
		boost::mpl::not_<
			traits::detail::is_fusion_vector<typename pk_tie<EntityType>::type>
		>,
		typename pk_tie<EntityType>::type
	>::type
	extract_pk(EntityType& entity) {
		BOOST_MPL_ASSERT((moneta::traits::has_pk<EntityType>));
		
		typename boost::mpl::at_c<
			typename boost::mpl::apply<detail::get_pk_memptr_types, EntityType>::type, 0
		>::type single_memptr;

		return single_memptr(entity);
	}

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
