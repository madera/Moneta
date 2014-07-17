#pragma once
#include "for_each_member.hxx"

namespace moneta { namespace codec {
	
	namespace detail {

		template <class Codec, class Iterator>
		struct encode_impl {
			struct state {
				Iterator& begin;
				Iterator& end;
				size_t total_written;
				bool good;

				state(Iterator& begin_, Iterator& end_)
				 : begin(begin_), end(end_), total_written(0), good(true) {}
			};

			state& _state;

			encode_impl(state& state)
			 : _state(state) {}

			template <class Entity, class Member, class Path>
			void operator()(const Entity& entity, Member member, Path path) const {
				if (_state.good) {
					typedef member_encoder<
						Codec,
						Member
					> member_encoder_type;

					int result = member_encoder_type()(
						entity, member, _state.begin, _state.end
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

	template <class Codec, class T, class Enable = void>
	struct encoder;

	template <class Codec, class Member>
	struct member_encoder {
		typedef typename Member::class_type entity_type;
		typedef typename Member::result_type value_type;

		template <class Iterator>
		int operator()(const entity_type& entity, Member member, Iterator begin, Iterator end) const {
			return encoder<Codec, value_type>()(member(entity), begin, end);
		}
	};

	template <class Codec, class Entity, class Iterator>
	int encode(const Entity& entity, Iterator begin, Iterator end) {
		typedef detail::encode_impl<Codec, Iterator> encoder_type;
		encoder_type::state state(begin, end);
		moneta::codec::for_each_member(entity, encoder_type(state));
		return state.total_written;
	}

}}
