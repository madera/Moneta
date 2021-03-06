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
#include "rtuple.hxx"
#include "detail/indices_of.hxx"
#include "detail/sub_tie_vector.hxx"
#include "detail/indices_of.hxx"

namespace moneta { namespace traits {

	template <class Entity>
	struct to_pk_rtuple_impl {
		typedef typename moneta::traits::rtuple<Entity>::type rtuple_type;

		typedef typename moneta::traits::detail::indices_of<
			typename moneta::traits::members<Entity>::type,
			moneta::traits::detail::is_pk<boost::mpl::_1>
		>::type indices_type;

		typedef moneta::traits::detail::sub_tie_vector<
			rtuple_type,
			indices_type
		> sub_tie_type;

		typedef typename moneta::traits::detail::deref_if_unary<
			typename sub_tie_type::type
		>::type type;

		type operator()(rtuple_type& rtuple) const {
			typename sub_tie_type::type sub = sub_tie_type()(rtuple);
			return moneta::traits::detail::deref_unary_vector(sub);
		}

		type operator()(const rtuple_type& rtuple) const {
			typename sub_tie_type::type sub = sub_tie_type()(rtuple);
			return moneta::traits::detail::deref_unary_vector(sub);
		}
	};

	template <class Entity>
	typename to_pk_rtuple_impl<Entity>::type
	to_pk_rtuple(typename moneta::traits::rtuple<Entity>::type& rtuple) {
		return to_pk_rtuple_impl<Entity>()(rtuple);
	}

	template <class Entity>
	typename to_pk_rtuple_impl<typename boost::add_const<Entity>::type>::type
	to_pk_rtuple(
		const typename moneta::traits::rtuple<
			typename boost::add_const<Entity>::type
		>::type& rtuple
	) {
		return to_pk_rtuple_impl<typename boost::add_const<Entity>::type>()(rtuple);
	}
}}
