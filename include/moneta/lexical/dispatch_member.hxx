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
#include "../limits/member.hxx"
#include "../traits/member_name.hxx"
#include "../traits/trait_array.hxx"
#include "../_aux/mplx_at_c.hxx"
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace moneta { namespace lexical {

	// TODO: Substitute this for dispatch_member_array_item().
	template <class Entity, class Operation>
	void dispatch_member(const std::string& name, const Operation& operation) {
		const int ordinal = traits::get_member_name_index<Entity>(name);
		mplx::at_c<typename moneta::traits::members<Entity>::type>(ordinal, operation);
	}

	template <class Entity, template <class T> class TypedTrait, class Operation>
	void dispatch_member_array_item(const std::string& name, const Operation& operation) {
		// FIXME: O(n): Sloooowwww.
		// XXX: Optimize this using hashes or something...

		typedef typename traits::trait_array<TypedTrait, Entity>::type array_type;
		array_type traits = traits::collect_member_traits<TypedTrait, Entity>();

		typename array_type::iterator itr = std::find(boost::begin(traits), boost::end(traits), name);
		BOOST_ASSERT(itr != boost::end(traits));

		const int ordinal = std::distance(boost::begin(traits), itr);
		mplx::at_c<typename moneta::traits::members<Entity>::type>(ordinal, operation);
	}

}}
