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
#include "pk.hxx"
#include "pk_tie.hxx"
#include "detail/memptr_constructor.hxx"
#include "detail/is_fusion_vector.hxx"
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/container/vector.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class Entity>
		typename pk_tie<Entity>::type
		pk_extractor(Entity& entity) {
			return memptr_constructor<
				typename pk_tie<Entity>::type,
				typename boost::mpl::apply<get_pk_memptr_types, Entity>::type,
				Entity,
				boost::mpl::size<
					typename pk_tie<Entity>::type
				>::value
			>()(entity);
		}

		template <class Entity>
		typename pk_tie<const Entity>::type
		pk_extractor(const Entity& entity) {
			return memptr_constructor<
				typename pk_tie<const Entity>::type,
				typename boost::mpl::apply<get_pk_memptr_types, Entity>::type,
				Entity,
				boost::mpl::size<
					typename pk_tie<const Entity>::type
				>::value
			>()(entity);
		}

	}

	template <class Entity>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk_tie<Entity>::type>,
		typename pk_tie<Entity>::type
	>::type
	extract_pk(Entity& entity) {
		BOOST_MPL_ASSERT((has_pk<Entity>));
		return detail::pk_extractor<Entity>(entity);
	}

	template <class Entity>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk_tie<const Entity>::type>,
		typename pk_tie<const Entity>::type
	>::type
	extract_pk(const Entity& entity) {
		BOOST_MPL_ASSERT((has_pk<Entity>));
		return detail::pk_extractor<Entity>(entity);
	}



	template <class Entity>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<Entity>::type> >,
		typename pk_tie<Entity>::type
	>::type
	extract_pk(Entity& entity) {
		BOOST_MPL_ASSERT((has_pk<Entity>));
	
		typename boost::mpl::at_c<
			typename boost::mpl::apply<detail::get_pk_memptr_types, Entity>::type,
			0
		>::type single_memptr;

		return single_memptr(entity);
	}

	template <class Entity>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<const Entity>::type> >,
		typename pk_tie<const Entity>::type
	>::type
	extract_pk(const Entity& entity) {
		BOOST_MPL_ASSERT((has_pk<Entity>));
	
		typename boost::mpl::at_c<
			typename boost::mpl::apply<detail::get_pk_memptr_types, Entity>::type,
			0
		>::type single_memptr;

		return single_memptr(entity);
	}

}}
