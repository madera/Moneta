#pragma once
#include "../algorithm/for_each_member.hxx"
#include "../algorithm/traverse.hxx"

namespace moneta { namespace codec {

	template <class Codec, class Path, class Entity, class Enable = void>
	struct enter_entity_encoder {};

	template <class Codec, class Member, class Path, class Enable = void>
	struct member_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return value_encoder<
				Codec, typename Member::result_type
			>()(Member()(entity), begin, end);
		}
	};

	template <class Codec, class T, class Enable = void>
	struct value_encoder;

	template <class Codec, class Path, class Entity, class Enable = void>
	struct leave_entity_encoder {};

	//

	template <class Iterator>
	struct encoder_state {
		typedef Iterator iterator_type;

		bool good;
		size_t total_written;

		Iterator& begin; // XXX
		Iterator& end; // XXX

		encoder_state(Iterator& begin_, Iterator& end_)
		 : begin(begin_), end(end_), total_written(0), good(true) {
		}
	};
	
	//

	template <class Codec, class Path, class Entity, class State>
	typename boost::enable_if<
		traits::detail::is_functor_callable<
			enter_entity_encoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
		>,
		int
	>::type
	attempt_enter(Entity& entity, State& state) {
		return enter_entity_encoder<Codec, Path, Entity>()(entity, state.begin, state.end);
	}

	template <class Codec, class Path, class Entity, class State>
	typename boost::disable_if<
		traits::detail::is_functor_callable<
			enter_entity_encoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
		>,
		int
	>::type
	attempt_enter(Entity& entity, State& state) {
		return 0;
	}

	template <class Codec, class Path, class Entity, class State>
	typename boost::enable_if<
		traits::detail::is_functor_callable<
			leave_entity_encoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
		>,
		int
	>::type
	attempt_leave(Entity& entity, State& state) {
		return leave_entity_encoder<Codec, Path, Entity>()(entity, state.begin, state.end);
	}

	template <class Codec, class Path, class Entity, class State>
	typename boost::disable_if<
		traits::detail::is_functor_callable<
			leave_entity_encoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
		>,
		int
	>::type
	attempt_leave(Entity& entity, State& state) {
		return 0;
	}

	namespace handlers {
		template <class Codec>
		struct enter : moneta::algorithm::traverse_enter {
			template <class Path, class Entity, class State>
			void operator()(Entity& entity, State& state) const {
				if (state.good) {
					int result = attempt_enter<Codec, Path>(entity, state);
					if (result > 0) {
						state.begin += result;
						state.total_written += result;
					} else if (result == 0) {
					} else {
						state.good = false;
						state.total_written = result;
					}
				}
			}
		};

		template <class Codec>
		struct member : moneta::algorithm::traverse_member {
			template <class Path, class Member, class Entity, class State>
			void operator()(Entity& entity, State& state) const {
				if (state.good) {
					int result = member_encoder<Codec, Member, Path>()(
						entity, state.begin, state.end
					);

					if (result > 0) {
						state.begin += result;
						state.total_written += result;
					} else if (result == 0) {
					} else {
						state.good = false;
						state.total_written = result;
					}
				}
			}
		};

		template <class Codec>
		struct leave : moneta::algorithm::traverse_leave {
			template <class Path, class Entity, class State>
			void operator()(Entity& entity, State& state) const {
				if (state.good) {
					int result = attempt_leave<Codec, Path>(entity, state);
					if (result > 0) {
						state.begin += result;
						state.total_written += result;
					} else if (result == 0) {
					} else {
						state.good = false;
						state.total_written = result;
					}
				}
			}
		};
	}

	template <class Codec, class Entity, class Enable = void>
	struct entity_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			encoder_state<Iterator> state(begin, end);

			moneta::algorithm::traverse<
				boost::mpl::vector3<
					handlers::enter<Codec>,
					handlers::member<Codec>,
					handlers::leave<Codec>
				>
			>(entity, state);

			return state.total_written;
		}
	};

	template <class Codec, class Entity, class Iterator>
	int encode(const Entity& entity, Iterator begin, Iterator end) {
		return entity_encoder<Codec, Entity>()(entity, begin, end);
	}
}}
