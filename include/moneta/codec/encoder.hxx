#pragma once
#include "../serialization/for_each_member.hxx"

namespace moneta { namespace codec {
	
	template <class Codec, class T, class Enable = void>
	struct encoder;

	namespace detail {

		template <class Codec, class Iterator>
		struct member_encoder {
			struct state {
				Iterator& begin;
				Iterator& end;
				size_t total_written;
				bool good;

				state(Iterator& begin_, Iterator& end_)
				 : begin(begin_), end(end_), total_written(0), good(true) {}
			};

			state& _state;

			member_encoder(state& state)
			 : _state(state) {}

			template <class Entity, class Member>
			void operator()(const Entity& entity, Member& member) const {
				if (_state.good) {
					int result = encoder<Codec, Member::result_type>()(
						member(entity), _state.begin, _state.end
					);

					if (result > 0) {
						_state.begin += result;
						_state.total_written += result;
					} else {
						_state.good = false;
						_state.total_written = result;
					}
				}
			}
		};

	}

	template <class Codec, class Entity, class Iterator>
	int encode(const Entity& entity, Iterator begin, Iterator end) {
		detail::member_encoder<Codec, Iterator>::state state(begin, end);
		moneta::serialization::for_each_member(entity, detail::member_encoder<Codec, Iterator>(state));
		return state.total_written;
	}

}}
