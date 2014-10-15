#pragma once
#include <boost/mpl/for_each.hpp>

namespace moneta { namespace algorithm {

	namespace detail {
	
		template <class Visitor, class Predicate>
		struct dispatch_entity_impl {

			struct state {
				bool done;
				Visitor& visitor;
				Predicate& predicate;

				state(Visitor& visitor_, Predicate& predicate_)
				 : done(false), visitor(visitor_), predicate(predicate_) {}
			};

			state& _state;

			dispatch_entity_impl(state& state)
			 : _state(state) {}

			template <class Entity>
			void operator()(const Entity&) const {
				if (!_state.done) {
					if (_state.predicate.operator()<Entity>()) {
						_state.visitor.operator()<Entity>();
						_state.done = true;
					}
				}
			}
		};

	}

	template <class Entities, class Visitor, class Predicate>
	Visitor& dispatch_entity(Visitor& visitor, Predicate& predicate) {
		BOOST_MPL_ASSERT((boost::mpl::is_sequence<Entities>));

		typedef detail::dispatch_entity_impl<Visitor, Predicate> operation;
		typename operation::state state(visitor, predicate);
		boost::mpl::for_each<Entities>(operation(state));
		return visitor;
	}

}}
