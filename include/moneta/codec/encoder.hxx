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

		template <class Actions, typename OutputIterator, class Substate>
		struct encoder_state {
			typedef Actions actions;
			typedef OutputIterator iterator_type;
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

			OutputIterator& next;
			OutputIterator end;

			bool good;
			int total_written;
			int last_result;
			
			Substate& substate;

			encoder_state(OutputIterator& next_, OutputIterator end_, Substate& substate_)
			 : next(next_), end(end_), good(true), total_written(0), last_result(0), substate(substate_) {
			}
		};
	}

	template <
		class Entity, class Path, class EncoderState,
		typename OutputIterator = typename EncoderState::iterator_type
	>
	class encoder_enter_or_leave_action {
		const Entity& _entity;
		EncoderState& _encoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_encoder_state.good = false;
			} else if (result > 0) {
				_encoder_state.total_written += result;
				// _encoder_state.next += result; // XXX
			} else {
			}

			_encoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					OutputIterator&, OutputIterator,
					const Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
					_entity
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					OutputIterator&, OutputIterator,
					const Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					const Entity&,
					const Path&,
					typename EncoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
					_entity,
					mplx::nullref<Path>(),
					_encoder_state.substate
				)
			);
		}

	public:
		encoder_enter_or_leave_action(const Entity& entity, EncoderState& encoder_state)
		 : _entity(entity), _encoder_state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_encoder_state.good) {
				process<Action>();
			}
		}
	};

	template <
		class Member,
		class Entity, class Path, class EncoderState,
		typename OutputIterator = typename EncoderState::iterator_type
	>
	class encoder_member_action {
		const Entity& _entity;
		EncoderState& _encoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_encoder_state.good = false;
			} else if (result > 0) {
				_encoder_state.total_written += result;
				// _encoder_state.next += result; // XXX
			} else {
			}

			_encoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					OutputIterator&, OutputIterator,
					const Member&,
					const Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					const Member&,
					const Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					const Member&,
					const Entity&,
					const Path&,
					typename EncoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_encoder_state.substate
				)
			);
		}
			
	public:
		encoder_member_action(const Entity& entity, EncoderState& encoder_state)
		 : _entity(entity), _encoder_state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_encoder_state.good) {
				process<Action>();
			}
		}
	};

	template <
		class Member,
		class Entity, class Path, class EncoderState,
		typename OutputIterator = typename EncoderState::iterator_type
	>
	class encoder_container_enter_or_leave_action {
		const Entity& _entity;
		EncoderState& _encoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_encoder_state.good = false;
			} else if (result > 0) {
				_encoder_state.total_written += result;
				// _encoder_state.next += result; // XXX
			} else {
			}

			_encoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					OutputIterator&, OutputIterator,
					const Member&,
					const Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					const Member&,
					const Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					const Member&,
					const Entity&,
					const Path&,
					typename EncoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_encoder_state.substate
				)
			);
		}

	public:
		encoder_container_enter_or_leave_action(const Entity& entity, EncoderState& encoder_state)
		 : _entity(entity), _encoder_state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};

	template <
		class Value, class Member,
		class Entity, class Path, class EncoderState,
		typename OutputIterator = typename EncoderState::iterator_type
	>
	class encoder_container_item_action {
		Value& _value;
		const Entity& _entity;
		EncoderState& _encoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_encoder_state.good = false;
			} else if (result > 0) {
				_encoder_state.total_written += result;
				//_encoder_state.next += result; // XXX
			} else {
			}

			_encoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					OutputIterator&, OutputIterator,
					Value&,
					const Member&,
					const Entity&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					Value&,
					const Member&,
					const Entity&,
					const Path&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
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
					OutputIterator&, OutputIterator,
					Value&,
					const Member&,
					const Entity&,
					const Path&,
					typename EncoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_encoder_state.next, _encoder_state.end,
					_value,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_encoder_state.substate
				)
			);
		}

	public:
		encoder_container_item_action(Value& value, const Entity& entity, EncoderState& encoder_state)
		 : _value(value), _entity(entity), _encoder_state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};

	struct encoder_enter_entity {
		template <class Entity, class Path, class EncoderState>
		void operator()(const Entity& entity, const Path&, EncoderState& encoder_state) const {
			boost::mpl::for_each<typename EncoderState::enter_actions>(
				encoder_enter_or_leave_action<Entity, Path, EncoderState>(entity, encoder_state)
			);
		}
	};

	struct encoder_member {
		template <class Member, class Entity, class Path, class EncoderState>
		void operator()(Member&, const Entity& entity, const Path&, EncoderState& encoder_state) const {
			boost::mpl::for_each<typename EncoderState::member_actions>(
				encoder_member_action<Member, Entity, Path, EncoderState>(entity, encoder_state)
			);
		}
	};

	struct encoder_leave_entity {
		template <class Entity, class Path, class EncoderState>
		void operator()(const Entity& entity, const Path&, EncoderState& encoder_state) const {
			boost::mpl::for_each<typename EncoderState::leave_actions>(
				encoder_enter_or_leave_action<Entity, Path, EncoderState>(entity, encoder_state)
			);
		}
	};

	struct encoder_enter_container {
		template <class Member, class Entity, class Path, class EncoderState>
		void operator()(Member&, const Entity& entity, const Path&, EncoderState& encoder_state) const {
			boost::mpl::for_each<typename EncoderState::enter_container_actions>(
				encoder_container_enter_or_leave_action<
					Member, Entity, Path, EncoderState
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_container_item {
		template <class Value, class Member, class Entity, class Path, class EncoderState>
		void operator()(
			Value& value, Member&, const Entity& entity, const Path&, EncoderState& encoder_state
		) const {
			boost::mpl::for_each<typename EncoderState::container_item_actions>(
				encoder_container_item_action<Value, Member, Entity, Path, EncoderState>(
					value, entity, encoder_state
				)
			);
		}
	};

	struct encoder_leave_container {
		template <class Member, class Entity, class Path, class EncoderState>
		void operator()(Member&, const Entity& entity, const Path&, EncoderState& encoder_state) const {
			boost::mpl::for_each<typename EncoderState::leave_container_actions>(
				encoder_container_enter_or_leave_action<
					Member, Entity, Path, EncoderState
				>(entity, encoder_state)
			);
		}
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct encoder {
		typedef encoder this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;

		template <class OutputIterator, class Entity, class Path, class State>
		int _encode(
			OutputIterator& next, OutputIterator end, const Entity& entity,
			Path* path = 0, State& state = State()
		) const {
			using namespace moneta::algorithm;

			typedef moneta::algorithm::traverse<
				enter_actions<encoder_enter_entity>,
				member_actions<encoder_member>,
				leave_actions<encoder_leave_entity>,
				enter_container_actions<encoder_enter_container>,
				container_item_actions<encoder_container_item>,
				leave_container_actions<encoder_leave_container>
			> traverser;

			detail::encoder_state<mpl_vector, OutputIterator, State> encoder_state(next, end, state);
			traverser()(entity, encoder_state);

			return (encoder_state.good)? encoder_state.total_written : encoder_state.last_result;
		}

	public:
		template <class OutputIterator, class Entity>
		int operator()(OutputIterator& next, OutputIterator end, const Entity& entity) {
			const boost::mpl::vector0<>* path = 0;
			const detail::no_state state = detail::no_state();
			return _encode(next, end, entity, path, state);
		}

		template <class OutputIterator, class Entity, class State>
		int operator()(OutputIterator& next, OutputIterator end, const Entity& entity, State& state) {
			const boost::mpl::vector0<>* path = 0;
			return _encode(next, end, entity, path, state);
		}
	};

	//
	// Syntax candy paused for now.
	//
	//template <class Encoder, class Entity, class Iterator>
	//int encode(Iterator next, Iterator end, const Entity& entity) {
	//	return Encoder()(next, end, entity);
	//}

	//template <class Encoder, class Entity, class Iterator, class State>
	//int encode(Iterator next, Iterator end, const Entity& entity, State state) {
	//	return Encoder()(next, end, entity, state);
	//}

}}
