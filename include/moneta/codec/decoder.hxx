#pragma once
#include "../serialization/for_each_member.hxx"
#include "../traits/fixed_values.hxx"

namespace moneta { namespace codec {
	
	template <class Codec, class T, class Enable = void>
	struct decoder;

	namespace detail {

		template <class Codec, class Member, class Enable = void>
		struct decode_member {
			template <class IteratorType>
			int operator()(typename Member::result_type& value, IteratorType begin, IteratorType end) const {
				return decoder<Codec, Member::result_type>()(value, begin, end);
			}
		};

		template <class Codec, class Member>
		struct decode_member<
			Codec,
			Member,
			typename boost::enable_if<traits::detail::fixed_value<Member> >::type
		> {
			template <class IteratorType>
			int operator()(typename Member::result_type& value, IteratorType begin, IteratorType end) const {
				int length = decoder<Codec, Member::result_type>()(value, begin, end);
				if (length >= 0) {
					if (value != traits::detail::fixed_value<Member>::get()) {
						return 0;
					}
				}

				return length;
			}
		};

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
					int result = decode_member<Codec, Member>()(
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
