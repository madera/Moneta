#pragma once
#include "../algorithm/traverse.hxx"

namespace moneta { namespace codec {

	template <class Codec, class Path, class Entity, class Enable = void>
	struct leave_entity_encoder {};


	template <class Codec, class T, class Enable = void>
	struct value_encoder;




	template <class Codec, class Entity, class Enable = void>
	struct entity_encoder;


	template <class Codec, class Path, class Entity, class Enable = void>
	struct enter_entity_encoder {};

	template <class Codec, class Member, class Path, class Enable = void>
	struct member_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			// Default behavior: Just pass the data.
			return value_encoder<Codec, typename Member::result_type>()(Member()(entity), begin, end);
		}
	};


	//
	// Container Members
	//

	template <class Codec, class Member, class Path, class Enable = void>
	struct enter_container_encoder {
		//template <class Entity, class Iterator>
		//int operator()(const Entity& entity, Iterator begin, Iterator end) const;
	};

	template <class Codec, class Member, class Path, class Enable = void>
	struct container_member_encoder {
		//template <class Entity, class Iterator>
		//int operator()(const Entity& entity, Iterator begin, Iterator end) const;
	};

	template <class Codec, class Member, class Path, class Enable = void>
	struct leave_container_encoder {
		//template <class Entity, class Iterator>
		//int operator()(const Entity& entity, Iterator begin, Iterator end) const;
	};



	namespace detail {

		template <class Iterator>
		struct encoder_state {
			typedef Iterator iterator_type;

			bool good;
			size_t total_written;

			Iterator& begin; // XXX
			Iterator& end; // XXX

			encoder_state(Iterator& begin_, Iterator& end_)
			 : begin(begin_), end(end_), total_written(0), good(true) {}
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
		attempt_enter_entity_encoder(Entity& entity, State& state) {
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
		attempt_enter_entity_encoder(Entity& entity, State& state) {
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
		attempt_leave_entity_encoder(Entity& entity, State& state) {
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
		attempt_leave_entity_encoder(Entity& entity, State& state) {
			return 0;
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <class Codec, class Path, class Entity, class State>
		typename boost::enable_if<
			traits::detail::is_functor_callable<
				enter_container_encoder<Codec, Path, Entity>,
				void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_enter_container_encoder(Entity& entity, State& state) {
			return enter_container_encoder<Codec, Path, Entity>()(entity, state.begin, state.end);
		}

		template <class Codec, class Path, class Entity, class State>
		typename boost::disable_if<
			traits::detail::is_functor_callable<
				enter_container_encoder<Codec, Path, Entity>,
				void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_enter_container_encoder(Entity& entity, State& state) {
			return 0;
		}

		template <class Codec, class Path, class Entity, class State>
		typename boost::enable_if<
			traits::detail::is_functor_callable<
				leave_container_encoder<Codec, Path, Entity>,
				void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_leave_container_encoder(Entity& entity, State& state) {
			return leave_container_encoder<Codec, Path, Entity>()(entity, state.begin, state.end);
		}

		template <class Codec, class Path, class Entity, class State>
		typename boost::disable_if<
			traits::detail::is_functor_callable<
				leave_container_encoder<Codec, Path, Entity>,
				void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_leave_container_encoder(Entity& entity, State& state) {
			return 0;
		}

		namespace encoder_handlers {
			template <class Codec>
			struct enter : moneta::algorithm::detail::traverse_enter {
				template <class Path, class Entity, class State>
				void operator()(Entity& entity, State& state) const {
					if (state.good) {
						int result = attempt_enter_entity_encoder<Codec, Path>(entity, state);
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
			struct member : moneta::algorithm::detail::traverse_member {
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
			struct leave : moneta::algorithm::detail::traverse_leave {
				template <class Path, class Entity, class State>
				void operator()(Entity& entity, State& state) const {
					if (state.good) {
						int result = attempt_leave_entity_encoder<Codec, Path>(entity, state);
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

			// XXX: FIXME: Finish this!!
			template <class Codec>
			struct enter_container : moneta::algorithm::detail::traverse_enter_container {
				template <class Member, class Path, class Entity, class State>
				void operator()(Entity& entity, State& state) const {
					if (state.good) {
						int result = attempt_enter_container_encoder<Codec, Path>(entity, state);
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
			struct container_member : moneta::algorithm::detail::traverse_container_member {
				template <class Member, class Path, class Entity, class State>
				void operator()(Entity& entity, State& state) const {
					if (state.good) {
						int result = container_member_encoder<Codec, Member, Path>()(
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

			// XXX: FIXME: Finish this!!
			template <class Codec>
			struct leave_container : moneta::algorithm::detail::traverse_leave_container {
				template <class Member, class Path, class Entity, class State>
				void operator()(Entity& entity, State& state) const {
					if (state.good) {
						int result = attempt_leave_container_encoder<Codec, Path>(entity, state);
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
	}

	template <class Codec, class Entity_, class Enable>
	struct entity_encoder {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			detail::encoder_state<Iterator> state(begin, end);

			moneta::algorithm::traverse<
				boost::mpl::vector6<
					detail::encoder_handlers::enter<Codec>,
					detail::encoder_handlers::member<Codec>,
					detail::encoder_handlers::enter_container<Codec>,
					detail::encoder_handlers::container_member<Codec>,
					detail::encoder_handlers::leave_container<Codec>,
					detail::encoder_handlers::leave<Codec>
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
