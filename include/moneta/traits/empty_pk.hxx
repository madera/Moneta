#pragma once
#include "pk.hxx"
#include "detail/memptr_constructor.hxx"
#include "detail/is_fusion_vector.hxx"
#include "detail/blanker.hxx"
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>

namespace moneta { namespace traits {

	template <class EntityType>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk<EntityType>::type>,
		typename pk<EntityType>::type
	>::type
	empty_pk() {
		typename pk<EntityType>::type result;
		return boost::fusion::transform(result, detail::blanker());
	}

	template <class EntityType>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<EntityType>::type> >,
		typename pk<EntityType>::type
	>::type
	empty_pk() {
		typename traits::pk<EntityType>::type result{};
		return result;
	}

	//template <class EntityType>
	//const bool has_empty_pk(const EntityType& entity) {
	//	return traits::extract_pk<EntityType>(entity) == empty_pk<EntityType>();
	//}

}}
