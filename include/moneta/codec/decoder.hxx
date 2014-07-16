#pragma once
#include "for_each_member.hxx"
#include "../traits/fixed_values.hxx"

namespace moneta { namespace codec {
	
	namespace detail {

		template <class Codec, class Member, class Enable = void>
		struct basic_member_decoder {
			typedef typename Member::class_type entity_type;

			template <class Iterator>
			int operator()(entity_type& entity, Member member, Iterator begin, Iterator end) const {
				return member_decoder<Codec, Member>()(entity, member, begin, end);
			}
		};

		template <class Codec, class Member>
		struct basic_member_decoder<
			Codec,
			Member,
			typename boost::enable_if<traits::detail::fixed_value<Member> >::type
		> {
			typedef typename Member::class_type entity_type;

			template <class Iterator>
			int operator()(entity_type& entity, Member member, Iterator begin, Iterator end) const {
				int length = member_decoder<Codec, Member>()(entity, member, begin, end);
				if (length >= 0) {
					if (member(entity) != traits::detail::fixed_value<Member>::get()) {
						return 0;
					}
				}

				return length;
			}
		};

		template <class Codec, class Iterator>
		struct decode_impl	 {
			struct state {
				Iterator& begin;
				Iterator& end;
				size_t total_read;
				bool good;

				state(Iterator& begin_, Iterator& end_)
				 : begin(begin_), end(end_), total_read(0), good(true) {}
			};

			state& _state;

			decode_impl(state& state)
			 : _state(state) {}

			template <class Entity, class Member, class Path>
			void operator()(Entity& entity, Member& member) const {
				if (!_state.good) {
					return;
				}

				int result = basic_member_decoder<Codec, Member>()(
					entity, member, _state.begin, _state.end
				);

				if (result > 0) {
					_state.begin += result;
					_state.total_read += result;
				} else {
					_state.good = false;
					_state.total_read = result;
				}
			}
		};

	}

	template < class Codec, class T, class Enable = void >
	struct decoder;

	template <class Codec, class Member>
	struct member_decoder {
		typedef typename Member::class_type entity_type;
		typedef typename Member::result_type value_type;

		template <class Iterator>
		int operator()(entity_type& entity, Member member, Iterator begin, Iterator end) const {
			return decoder<Codec, value_type>()(member(entity), begin, end);
		}
	};

	template <class Codec, class Entity, class Iterator>
	int decode(Entity& entity, Iterator begin, Iterator end) {
		detail::decode_impl<Codec, Iterator>::state state(begin, end);
		moneta::codec::for_each_member(entity, detail::decode_impl<Codec, Iterator>(state));
		return state.total_read;
	}

}}
