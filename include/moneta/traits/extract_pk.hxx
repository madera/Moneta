#pragma once
#include "pk.hxx"
#include "pk_tie.hxx"
#include "detail/memptr_constructor.hxx"
#include "detail/is_fusion_vector.hxx"
#include <boost/fusion/include/equal_to.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityType>
		typename pk_tie<EntityType>::type
		pk_extractor(EntityType& entity) {
			return memptr_constructor<
				typename pk_tie<EntityType>::type,
				typename boost::mpl::apply<get_pk_memptr_types, EntityType>::type,
				EntityType,
				boost::mpl::size<
					typename pk_tie<EntityType>::type
				>::value
			>()(entity);
		}

		template <class EntityType>
		typename const_pk_tie<EntityType>::type
		pk_extractor(const EntityType& entity) {
			return memptr_constructor<
				typename const_pk_tie<EntityType>::type,
				typename boost::mpl::apply<get_pk_memptr_types, EntityType>::type,
				EntityType,
				boost::mpl::size<
					typename const_pk_tie<EntityType>::type
				>::value
			>()(entity);
		}

	}

	template <class EntityType>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk_tie<EntityType>::type>,
		typename pk_tie<EntityType>::type
	>::type
	extract_pk(EntityType& entity) {
		BOOST_MPL_ASSERT((has_pk<EntityType>));
		return detail::pk_extractor<EntityType>(entity);
	}

	template <class EntityType>
	typename boost::enable_if<
		detail::is_fusion_vector<typename const_pk_tie<EntityType>::type>,
		typename const_pk_tie<EntityType>::type
	>::type
	extract_pk(const EntityType& entity) {
		BOOST_MPL_ASSERT((has_pk<EntityType>));
		return detail::pk_extractor<EntityType>(entity);
	}



	template <class EntityType>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<EntityType>::type> >,
		typename pk_tie<EntityType>::type
	>::type
	extract_pk(EntityType& entity) {
		BOOST_MPL_ASSERT((has_pk<EntityType>));
	
		typename boost::mpl::at_c<
			typename boost::mpl::apply<detail::get_pk_memptr_types, EntityType>::type,
			0
		>::type single_memptr;

		return single_memptr(entity);
	}

	template <class EntityType>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename const_pk_tie<EntityType>::type> >,
		typename const_pk_tie<EntityType>::type
	>::type
	extract_pk(const EntityType& entity) {
		BOOST_MPL_ASSERT((has_pk<EntityType>));
	
		typename boost::mpl::at_c<
			typename boost::mpl::apply<detail::get_pk_memptr_types, EntityType>::type,
			0
		>::type single_memptr;

		return single_memptr(entity);
	}

}}
