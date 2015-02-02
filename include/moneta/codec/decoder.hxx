#pragma once
#include "../algorithm/traverse.hxx"

// TODO: Put in detail, well, the detail impls...

namespace moneta { namespace codec {

	using moneta::algorithm::enter_actions;
	using moneta::algorithm::member_actions;
	using moneta::algorithm::leave_actions;
	using moneta::algorithm::enter_container_actions;
	using moneta::algorithm::container_item_actions;
	using moneta::algorithm::leave_container_actions;

	namespace detail {
		using namespace moneta::algorithm::detail;

		template <class Actions, typename InputIterator, class Substate>
		struct decoder_state {
			typedef Actions actions;
			typedef InputIterator iterator_type;
			typedef Substate substate_type;

			//
			//
			//
			typedef typename detail::actions_of<
				Actions,
				detail::traverse_enter
			>::type enter_actions;
			
			typedef typename detail::actions_of<
				Actions,
				detail::traverse_member
			>::type member_actions;

			typedef typename detail::actions_of<
				Actions,
				detail::traverse_leave
			>::type leave_actions;

			typedef typename detail::actions_of<
				Actions,
				detail::traverse_enter_container
			>::type enter_container_actions;

			typedef typename detail::actions_of<
				Actions,
				detail::traverse_container_item
			>::type container_item_actions;

			typedef typename detail::actions_of<
				Actions,
				detail::traverse_leave_container
			>::type leave_container_actions;

			//
			//
			//

			InputIterator& next;
			InputIterator end;

			bool good;
			int total_written;
			int last_result;
			
			Substate& substate;

			decoder_state(InputIterator& next_, InputIterator end_, Substate& substate_)
			 : next(next_), end(end_), good(true), total_written(0), last_result(0), substate(substate_) {
			}
		};
	}

	template <
		class Entity, class Path, class DecoderState,
		typename InputIterator = typename DecoderState::iterator_type
	>
	class decoder_enter_or_leave_action {
		Entity& _entity;
		DecoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					_entity
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					_entity,
					mplx::nullref<Path>()
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					Entity&,
					const Path&,
					typename DecoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}

	public:
		decoder_enter_or_leave_action(Entity& entity, DecoderState& decoder_state)
		 : _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_decoder_state.good) {
				process<Action>();
			}
		}
	};

	template <class Member, class Entity, class Path, class DecoderState, typename InputIterator = typename DecoderState::iterator_type>
	class decoder_member_action {
		Entity& _entity;
		DecoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					const Member&,
					Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					const Member&,
					Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>()
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					const Member&,
					Entity&,
					const Path&,
					typename DecoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}
			
	public:
		decoder_member_action(Entity& entity, DecoderState& decoder_state)
		 : _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_decoder_state.good) {
				process<Action>();
			}
		}
	};

	template <class Member, class Entity, class Path, class DecoderState, typename InputIterator = typename DecoderState::iterator_type>
	class decoder_container_enter_or_leave_action {
		Entity& _entity;
		DecoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					const Member&,
					Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					const Member&,
					Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>()
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					const Member&,
					Entity&,
					const Path&,
					typename DecoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}

	public:
		decoder_container_enter_or_leave_action(Entity& entity, DecoderState& decoder_state)
		 : _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};

	template <class Value, class Member, class Entity, class Path, class DecoderState, typename InputIterator = typename DecoderState::iterator_type>
	class decoder_container_item_action {
		Value& _value;
		Entity& _entity;
		DecoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					Value&,
					const Member&,
					Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					_value,
					mplx::nullref<Member>(),
					_entity
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					Value&,
					const Member&,
					Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					_value,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>()
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					InputIterator&, InputIterator,
					Value&,
					const Member&,
					Entity&,
					const Path&,
					typename DecoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.next, _decoder_state.end,
					_value,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}

	public:
		decoder_container_item_action(Value& value, Entity& entity, DecoderState& decoder_state)
		 : _value(value), _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};
	
	struct decoder_enter_entity {
		template <class Entity, class Path, class DecoderState>
		void operator()(Entity& entity, const Path&, DecoderState& decoder_state) const {
			boost::mpl::for_each<typename DecoderState::enter_actions>(
				decoder_enter_or_leave_action<Entity, Path, DecoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_member {
		template <class Member, class Entity, class Path, class DecoderState>
		void operator()(Member&, Entity& entity, const Path&, DecoderState& decoder_state) const {
			boost::mpl::for_each<typename DecoderState::member_actions>(
				decoder_member_action<Member, Entity, Path, DecoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_leave_entity {
		template <class Entity, class Path, class DecoderState>
		void operator()(Entity& entity, const Path&, DecoderState& decoder_state) const {
			boost::mpl::for_each<typename DecoderState::leave_actions>(
				decoder_enter_or_leave_action<Entity, Path, DecoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_enter_container {
		template <class Member, class Entity, class Path, class DecoderState>
		void operator()(Member&, Entity& entity, const Path&, DecoderState& decoder_state) const {
			boost::mpl::for_each<typename DecoderState::enter_container_actions>(
				decoder_container_enter_or_leave_action<
					Member, Entity, Path, DecoderState
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_container_item {
		template <class Value, class Member, class Entity, class Path, class DecoderState>
		void operator()(Value& value, Member&, Entity& entity, const Path&, DecoderState& decoder_state) const {
			boost::mpl::for_each<typename DecoderState::container_item_actions>(
				decoder_container_item_action<Value, Member, Entity, Path, DecoderState>(
					value, entity, decoder_state
				)
			);
		}
	};

	struct decoder_leave_container {
		template <class Member, class Entity, class Path, class DecoderState>
		void operator()(Member&, Entity& entity, const Path&, DecoderState& decoder_state) const {
			boost::mpl::for_each<typename DecoderState::leave_container_actions>(
				decoder_container_enter_or_leave_action<
					Member, Entity, Path, DecoderState
				>(entity, decoder_state)
			);
		}
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct decoder {
		typedef decoder this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;

		template <class InputIterator, class Entity, class Path, class State>
		int _decode(
			InputIterator& next, InputIterator end, Entity& entity,
			Path* path = 0, State& state = State()
		) const {
			using namespace moneta::algorithm;

			typedef moneta::algorithm::traverse<
				enter_actions<decoder_enter_entity>,
				member_actions<decoder_member>,
				leave_actions<decoder_leave_entity>,
				enter_container_actions<decoder_enter_container>,
				container_item_actions<decoder_container_item>,
				leave_container_actions<decoder_leave_container>
			> traverser;

			detail::decoder_state<mpl_vector, InputIterator, State> decoder_state(next, end, state);
			traverser()(entity, decoder_state);

			return (decoder_state.good)? decoder_state.total_written : decoder_state.last_result;
		}

	public:
		template <class InputIterator, class Entity>
		int operator()(InputIterator& next, InputIterator end, Entity& entity) {
			const boost::mpl::vector0<>* path = 0;
			const detail::no_state state = detail::no_state();
			return _decode(next, end, entity, path, state);
		}

		template <class InputIterator, class Entity, class State>
		int operator()(InputIterator& next, InputIterator end, Entity& entity, State& state) {
			const boost::mpl::vector0<>* path = 0;
			return _decode(next, end, entity, path, state);
		}
	};

	//
	// Syntax candy
	//
	template <class Decoder, class InputIterator, class Entity>
	typename boost::enable_if<traits::is_entity<Entity>, int>::type
	decode(InputIterator next, InputIterator end, Entity& entity) {
		return Decoder()(next, end, entity);
	}

	template <class Decoder, class InputIterator, class Entity, class State>
	typename boost::enable_if<traits::is_entity<Entity>, int>::type
	decode(InputIterator next, InputIterator end, Entity& entity, State& state) {
		return Decoder()(next, end, entity, state);
	}

	template <class Decoder, class InputIterator, class Visitor>
	typename boost::disable_if<traits::is_entity<Visitor>, int>::type
	decode(InputIterator next, InputIterator end, Visitor& visitor) {
		return Decoder()(next, end, visitor);
	}

	template <class Decoder, class InputIterator, class Visitor, class State>
	typename boost::disable_if<traits::is_entity<Visitor>, int>::type
	decode(InputIterator next, InputIterator end, Visitor& visitor, State& state) {
		return Decoder()(next, end, visitor, state);
	}

}}
