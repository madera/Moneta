#pragma once
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
				if (!_state.done) {
#ifdef BOOST_MSVC
					if (_state.predicate.operator()<Entity>()) {
						_state.visitor.operator()<Entity>();
#else
					if (_state.predicate.template operator()<Entity>()) {
						_state.visitor.template operator()<Entity>();
#endif
						_state.done = true;
					}
				}
			}
		};

	}

	template <class Entities, class Visitor, class Predicate>
	const Visitor& dispatch_entity(const Visitor& visitor, const Predicate& predicate) {
		BOOST_MPL_ASSERT((boost::mpl::is_sequence<Entities>));

		typedef detail::dispatch_entity_impl<Visitor, Predicate> operation;
		typename operation::state state(visitor, predicate);
		boost::mpl::for_each<Entities>(operation(state));
		return visitor;
	}

}}
