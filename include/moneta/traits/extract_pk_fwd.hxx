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
#include "pk_tie_fwd.hxx"
#include "detail/is_fusion_vector.hxx"

namespace moneta { namespace traits {

	template <class Entity>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk_tie<Entity>::type>,
		typename pk_tie<Entity>::type
	>::type
	extract_pk(Entity& entity);
	
	template <class Entity>
	typename boost::enable_if<
		detail::is_fusion_vector<typename pk_tie<const Entity>::type>,
		typename pk_tie<const Entity>::type
	>::type
	extract_pk(const Entity& entity);
	
	template <class Entity>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<Entity>::type> >,
		typename pk_tie<Entity>::type
	>::type
	extract_pk(Entity& entity);

	template <class Entity>
	typename boost::enable_if<
		boost::mpl::not_<detail::is_fusion_vector<typename pk_tie<const Entity>::type> >,
		typename pk_tie<const Entity>::type
	>::type
	extract_pk(const Entity& entity);

}}
