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
#include "../detail/keywords.hxx"
#include <boost/mpl/for_each.hpp>

namespace moneta { namespace algorithm {

	namespace detail {
	
		template <class Visitor, class Predicate>
		struct dispatch_entity_impl {

			struct state {
				bool done;
				const Visitor& visitor;
				const Predicate& predicate;

				state(const Visitor& visitor_, const Predicate& predicate_)
				 : done(false), visitor(visitor_), predicate(predicate_) {}
			};

			state& _state;

			dispatch_entity_impl(state& state)
			 : _state(state) {}

			template <class Entity>
			void operator()(const Entity&) const {
				Entity* dummy = 0;

				if (!_state.done) {
					if (_state.predicate(dummy)) {
						_state.visitor(dummy);
						_state.done = true;
					}
				}
			}
		};

	}

	//
	// XXX: Keep an eye on this. Being O(n) could be a bottleneck.
	//
	template <class Entities, class Visitor, class Predicate>
	const Visitor& dispatch_entity(const Visitor& visitor, const Predicate& predicate) {
		BOOST_MPL_ASSERT((boost::mpl::is_sequence<Entities>));

		typedef detail::dispatch_entity_impl<Visitor, Predicate> operation;
		typename operation::state state(visitor, predicate);
		boost::mpl::for_each<Entities>(operation(state));
		return visitor;
	}

}}
