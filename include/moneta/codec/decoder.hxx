#pragma once
#include "../serialization/for_each_member.hxx"

namespace moneta { namespace codec {
	
	template <class Codec, class T, class Enable = void>
	struct decoder;

	namespace detail {

		template <class Codec, class Iterator>
		struct member_decoder	 {
			struct state {
				Iterator& begin;
				Iterator& end;
				size_t total_read;
				bool good;

				state(Iterator& begin_, Iterator& end_)
				 : begin(begin_), end(end_), total_read(0), good(true) {}
			};

			state& _state;

			member_decoder(state& state)
			 : _state(state) {}

			template <class Entity, class Member>
			void operator()(Entity& entity, Member& member) const {
				if (_state.good) {
					int result = decoder<Codec, Member::result_type>()(
						member(entity), _state.begin, _state.end
					);

					if (result > 0) {
						_state.begin += result;
						_state.total_read += result;
					} else {
						_state.good = false;
						_state.total_read = result;
					}
				}
			}
		};

	}

	template <class Codec, class Entity, class Iterator>
	int decode(Entity& entity, Iterator begin, Iterator end) {
		detail::member_decoder<Codec, Iterator>::state state(begin, end);
		moneta::serialization::for_each_member(entity, detail::member_decoder<Codec, Iterator>(state));
		return state.total_read;
	}

}}
