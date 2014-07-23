#pragma once
#include "../algorithm/for_each_member.hxx"

namespace moneta { namespace codec {
	
	template <class Codec, class T, class Enable = void>
	struct encoder;

	// Default implementation.
	//
	template <class Codec, class Member, class Path>
	struct member_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Member& member, Iterator& begin, Iterator& end) const {
			return detail::apply_encoder<Codec, Member, Path>(entity, member, begin, end);
		}
	};

	namespace detail {

		template <class Codec, class Member, class Path, class Entity, class Iterator>
		typename boost::enable_if<
			traits::detail::is_functor_callable<
				encoder<Codec, typename Member::result_type>,
				int (typename Member::result_type, Iterator, Iterator, Path)
			>,
			int
		>::type
		apply_encoder(Entity& entity, Member& member, Iterator& begin, Iterator& end) {
			//BOOST_MPL_ASSERT((boost::is_same<Entity, typename Member::class_type>));
			return encoder<Codec, typename Member::result_type>()(member(entity), begin, end, Path());
		}

		template <class Codec, class Member, class Path, class Entity, class Iterator>
		typename boost::enable_if <
			traits::detail::is_functor_callable<
				encoder<Codec, typename Member::result_type>,
				int(typename Member::result_type, Iterator, Iterator)
			>,
			int
		>::type
		apply_encoder(Entity& entity, Member& member, Iterator& begin, Iterator& end) {
			//BOOST_MPL_ASSERT((boost::is_same<Entity, typename Member::class_type>));
			return encoder<Codec, typename Member::result_type>()(member(entity), begin, end);
		}

		template <class Codec, class Iterator>
		struct encode_impl {
			struct state {
				Iterator begin;
				Iterator end;
				size_t total_written;
				bool good;

				state(Iterator begin_, Iterator end_)
				 : begin(begin_), end(end_), total_written(0), good(true) {}
			};

			state& _state;

			encode_impl(state& state)
			 : _state(state) {}

			template <class Entity, class Member, class Path>
			void operator()(Entity& entity, Member& member, Path& path) const {
				if (_state.good) {
					int result = member_encoder<Codec, Member, Path>()(
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

	template <class Codec, class Entity, class Iterator>
	int encode(const Entity& entity, Iterator begin, Iterator end) {
		typedef detail::encode_impl<Codec, Iterator> encoder_type;
		encoder_type::state state(begin, end);
		moneta::algorithm::for_each_member(entity, encoder_type(state));
		return state.total_written;
	}

}}
