#pragma once
#include "../algorithm/for_each_member.hxx"

namespace moneta { namespace codec {
	
	template <class Codec, class T, class Enable = void>
	struct value_encoder;

	template <class Codec, class Member, class Path, class Enable = void>
	struct member_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Member& member, Iterator& begin, Iterator& end) const {
			return detail::apply_value_encoder<Codec, Member, Path>(entity, member, begin, end);
		}
	};

	namespace detail {

		// Apply value_encoder with Path
		//
		template <class Codec, class Member, class Path, class Entity, class Iterator>
		typename boost::enable_if<
			traits::detail::is_functor_callable<
				value_encoder<Codec, typename Member::result_type>,
				int (typename Member::result_type, Iterator, Iterator, Path)
			>,
			int
		>::type
		apply_value_encoder(Entity& entity, Member& member, Iterator& begin, Iterator& end) {
			//BOOST_MPL_ASSERT((boost::is_same<Entity, typename Member::class_type>));
			return value_encoder<Codec, typename Member::result_type>()(member(entity), begin, end, Path());
		}

		// Apply value_encoder without Path
		//
		template <class Codec, class Member, class Path, class Entity, class Iterator>
		typename boost::enable_if <
			traits::detail::is_functor_callable<
				value_encoder<Codec, typename Member::result_type>,
				int(typename Member::result_type, Iterator, Iterator)
			>,
			int
		>::type
		apply_value_encoder(Entity& entity, Member& member, Iterator& begin, Iterator& end) {
			//BOOST_MPL_ASSERT((boost::is_same<Entity, typename Member::class_type>));
			return value_encoder<Codec, typename Member::result_type>()(member(entity), begin, end);
		}

		// Default entity encoder implementation.
		//
		// State is a separate class because operator() must be const.
		//
		template <class Codec, class Iterator>
		struct default_entity_encoder {
			struct state {
				Iterator begin;
				Iterator end;
				size_t total_written;
				bool good;

				state(Iterator begin_, Iterator end_)
				 : begin(begin_), end(end_), total_written(0), good(true) {}
			};

			state& _state;

			default_entity_encoder(state& state)
			 : _state(state) {}

			template <class Entity, class Path>
			void enter(Entity& entity) const {
				//std::cerr << "e:" << moneta::traits::get_entity_name<Entity>() << std::endl;
			}

			template <class Entity, class Member, class Path>
			void operator()(Entity& entity) const {
				if (_state.good) {
					int result = member_encoder<Codec, Member, Path>()(
						entity, Member(), _state.begin, _state.end
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

			template <class Entity, class Path>
			void leave(Entity& entity) const {
				//std::cerr << "l:" << moneta::traits::get_entity_name<Entity>() << std::endl;
			}
		};

	}

	template <class Codec, class Entity, class Enable = void>
	struct entity_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			typedef detail::default_entity_encoder<Codec, Iterator> encoder_type;
			encoder_type::state state(begin, end);
			moneta::algorithm::for_each_member(entity, encoder_type(state));
			return state.total_written;
		}
	};

	template <class Codec, class Entity, class Iterator>
	int encode(const Entity& entity, Iterator begin, Iterator end) {
		return entity_encoder<Codec, Entity>()(entity, begin, end);
	}

}}
