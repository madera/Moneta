#pragma once
#include "pk_tie.hxx"
#include "extract_pk.hxx"
#include "detail/memptr_constructor.hxx"
#include "detail/is_fusion_vector.hxx"
#include "detail/blanker.hxx"
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>

namespace moneta { namespace traits {

	template <class Entity>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk<Entity>::type>,
		typename pk<Entity>::type
	>::type
	empty_pk() {
		typename pk<Entity>::type result;
		return boost::fusion::transform(result, detail::blanker());
	}

	template <class Entity>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<Entity>::type> >,
		typename pk<Entity>::type
	>::type
	empty_pk() {
		typedef typename traits::pk<Entity>::type result_type;
		result_type result = result_type();
		return result;
	}

	template <class Entity>
	bool has_empty_pk(const Entity& entity) {
		return traits::extract_pk<Entity>(entity) == empty_pk<Entity>();
	}

}}
