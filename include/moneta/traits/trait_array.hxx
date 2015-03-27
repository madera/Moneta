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
#include "members.hxx"
#include <boost/array.hpp>

namespace moneta { namespace traits {

	template <
		template <class Entity>
		class TypedTrait,

		class Entity
	>
	struct trait_array : boost::mpl::identity<
		boost::array<
			typename TypedTrait<Entity>::trait_type,
			boost::mpl::size<
				typename members<Entity>::type
			>::value
		>
	> {};

	namespace detail {

		template <
			template <class Entity>
			class TypedTrait,
		
			class Entity
		>
		struct trait_array_assigner {
			typedef typename trait_array<TypedTrait, Entity>::type array_type;

			struct state {
				array_type result;
				int _current_pos;

				state()
				 : _current_pos(0) {}
			};

			state& _state;

			trait_array_assigner(state& state)
			 : _state(state) {}

			template <class Member>
			void operator()(Member) const {
				_state.result[_state._current_pos++] = TypedTrait<Member>::get();
			}
		};

	}

	template <
		template <class Entity>
		class TypedTrait,
		
		class Entity
	>
	typename trait_array<TypedTrait, Entity>::type
	collect_member_traits() {
		typedef typename detail::trait_array_assigner<TypedTrait, Entity> assigner_type;
		typename assigner_type::state state;
		boost::fusion::for_each(typename members<Entity>::type(), assigner_type(state));
		return state.result;
	}

}}
