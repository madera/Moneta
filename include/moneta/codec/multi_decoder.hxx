#pragma once
#include <boost/variant.hpp>

namespace moneta { namespace codec {
	
	namespace detail {

		template <class EntityType, class IteratorType>
		struct attempt_decode {

			struct state {
				IteratorType& begin;
				IteratorType& end;
				EntityType& entity;
				bool done;

				state(IteratorType& begin_, IteratorType& end_, EntityType& entity_)
				 : begin(begin_), end(end_), entity(entity_), done(false) {}
			};

			state& _state;

			attempt_decode(state& state)
			 : _state(state) {}

			template <class EntityType>
			void operator()(EntityType& ignored) const {
				if (!_state.done) {
					EntityType entity;
					int result = rawbin::decoder<EntityType>()(_state.begin, _state.end, entity);
					if (result > 0) {
						_state.entity = entity;
						_state.done = true;
					}
				}
			}

		};

	}

	template <class EntityTypeSequence>
	struct multi_decoder {
		typedef boost::variant<EntityTypeSequence> variant_type;

		template <class IteratorType>
		int operator()(IteratorType begin, IteratorType end, variant_type& result) const {
			detail::multi_decoder_decode::state state(result);
			detail::multi_decoder_decode decoder(state);
			boost::mpl::for_each<EntityTypeSequence>(decoder);

			if (state.result == 0) {
				return state.entity;
			} else {
				return state.result;
			}
		}
	};

}}
