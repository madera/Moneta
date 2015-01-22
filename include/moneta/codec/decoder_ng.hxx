#pragma once
#include "../algorithm/traverse_ng.hxx"
//#include "../traits/member_names.hxx"

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

		template <class Actions, typename Iterator, class Substate>
		struct decoder_state {
			typedef Actions actions;
			typedef Iterator iterator_type;
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

			Iterator begin;
			Iterator end;

			bool good;
			int total_written;
			int last_result;
			
			Substate& substate;

			decoder_state(Iterator begin_, Iterator end_, Substate& substate_)
			 : begin(begin_), end(end_), good(true), total_written(0), last_result(0), substate(substate_) {
			}
		};
	}

	template <class Entity, class Path, class decoderState, typename Iterator = typename decoderState::iterator_type>
	class decoder_enter_or_leave_action {
		Entity& _entity;
		decoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
				_decoder_state.begin += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&)
			>
		>::type
		process() const {
			update_result(Action()(_decoder_state.begin, _decoder_state.end, _entity));
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, const Path&)
			>
		>::type
		process() const {
			update_result(
				Action()(_decoder_state.begin, _decoder_state.end, _entity, mplx::nullref<Path>())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (
					Iterator, Iterator,
					Entity&,
					const Path&,
					typename decoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
					_entity, mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}

	public:
		decoder_enter_or_leave_action(Entity& entity, decoderState& decoder_state)
		 : _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_decoder_state.good) {
				process<Action>();
			}
		}
	};

	template <class Member, class Entity, class Path, class decoderState, typename Iterator = typename decoderState::iterator_type>
	class decoder_member_action {
		Entity& _entity;
		decoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
				_decoder_state.begin += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, const Member&, Entity&)
			>
		>::type
		process() const {
			update_result(
				Action()(_decoder_state.begin, _decoder_state.end, mplx::nullref<Member>(), _entity)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, const Member&, Entity&, const Path&)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
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
					Iterator, Iterator,
					const Member&,
					Entity&,
					const Path&,
					typename decoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}
			
	public:
		decoder_member_action(Entity& entity, decoderState& decoder_state)
		 : _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_decoder_state.good) {
				process<Action>();
			}
		}
	};

	template <class Member, class Entity, class Path, class decoderState, typename Iterator = typename decoderState::iterator_type>
	class decoder_container_enter_or_leave_action {
		Entity& _entity;
		decoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
				_decoder_state.begin += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, const Member&, Entity&)
			>
		>::type
		process() const {
			update_result(
				Action()(_decoder_state.begin, _decoder_state.end, mplx::nullref<Member>(), _entity)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, const Member&, Entity&, const Path&)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
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
					Iterator, Iterator,
					const Member&,
					Entity&,
					const Path&,
					typename decoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}

	public:
		decoder_container_enter_or_leave_action(Entity& entity, decoderState& decoder_state)
		 : _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};

	template <class Value, class Member, class Entity, class Path, class decoderState, typename Iterator = typename decoderState::iterator_type>
	class decoder_container_item_action {
		Value& _value;
		Entity& _entity;
		decoderState& _decoder_state;

		void update_result(const int result) const {
			if (result < 0) {
				_decoder_state.good = false;
			} else if (result > 0) {
				_decoder_state.total_written += result;
				_decoder_state.begin += result;
			} else {
			}

			_decoder_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Value&, const Member&, Entity&)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
					_value, mplx::nullref<Member>(), _entity
				)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Value&, const Member&, Entity&, const Path&)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
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
					Iterator, Iterator,
					Value&,
					const Member&,
					Entity&,
					const Path&,
					typename decoderState::substate_type&
				)
			>
		>::type
		process() const {
			update_result(
				Action()(
					_decoder_state.begin, _decoder_state.end,
					_value,
					mplx::nullref<Member>(),
					_entity,
					mplx::nullref<Path>(),
					_decoder_state.substate
				)
			);
		}

	public:
		decoder_container_item_action(Value& value, Entity& entity, decoderState& decoder_state)
		 : _value(value), _entity(entity), _decoder_state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};
	
	struct decoder_enter_entity {
		template <class Entity, class Path, class decoderState>
		void operator()(Entity& entity, const Path&, decoderState& decoder_state) const {
			boost::mpl::for_each<typename decoderState::enter_actions>(
				decoder_enter_or_leave_action<Entity, Path, decoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_member {
		template <class Member, class Entity, class Path, class decoderState>
		void operator()(Member&, Entity& entity, const Path&, decoderState& decoder_state) const {
			boost::mpl::for_each<typename decoderState::member_actions>(
				decoder_member_action<Member, Entity, Path, decoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_leave_entity {
		template <class Entity, class Path, class decoderState>
		void operator()(Entity& entity, const Path&, decoderState& decoder_state) const {
			boost::mpl::for_each<typename decoderState::leave_actions>(
				decoder_enter_or_leave_action<Entity, Path, decoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_enter_container {
		template <class Member, class Entity, class Path, class decoderState>
		void operator()(Member&, Entity& entity, const Path&, decoderState& decoder_state) const {
			boost::mpl::for_each<typename decoderState::enter_container_actions>(
				decoder_container_enter_or_leave_action<Member, Entity, Path, decoderState>(entity, decoder_state)
			);
		}
	};

	struct decoder_container_item {
		template <class Value, class Member, class Entity, class Path, class decoderState>
		void operator()(Value& value, Member&, Entity& entity, const Path&, decoderState& decoder_state) const {
			boost::mpl::for_each<typename decoderState::container_item_actions>(
				decoder_container_item_action<Value, Member, Entity, Path, decoderState>(
					value, entity, decoder_state
				)
			);
		}
	};

	struct decoder_leave_container {
		template <class Member, class Entity, class Path, class decoderState>
		void operator()(Member&, Entity& entity, const Path&, decoderState& decoder_state) const {
			boost::mpl::for_each<typename decoderState::leave_container_actions>(
				decoder_container_enter_or_leave_action<Member, Entity, Path, decoderState>(entity, decoder_state)
			);
		}
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct decoder {
		typedef decoder this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;

		template <class Iterator, class Entity, class Path, class State>
		int _decode(Iterator begin, Iterator end, Entity& entity, Path* path = 0, State& state = State()) const {
			using namespace moneta::algorithm;

			typedef moneta::algorithm::traverse<
				enter_actions<decoder_enter_entity>,
				member_actions<decoder_member>,
				leave_actions<decoder_leave_entity>,
				enter_container_actions<decoder_enter_container>,
				container_item_actions<decoder_container_item>,
				leave_container_actions<decoder_leave_container>
			> traverser;

			detail::decoder_state<mpl_vector, Iterator, State> decoder_state(begin, end, state);
			traverser()(entity, decoder_state);

			return (decoder_state.good)? decoder_state.total_written : decoder_state.last_result;
		}

	public:
		template <class Iterator, class Entity>
		int operator()(Iterator begin, Iterator end, Entity& entity) {
			const boost::mpl::vector0<>* path = 0;
			const detail::no_state state = detail::no_state();
			return _decode(begin, end, entity, path, state);
		}

		template <class Iterator, class Entity, class State>
		int operator()(Iterator begin, Iterator end, Entity& entity, State& state) {
			const boost::mpl::vector0<>* path = 0;
			return _decode(begin, end, entity, path, state);
		}
	};

	//
	// Syntax candy paused for now.
	//
	//template <class decoder, class Entity, class Iterator>
	//int decode(Iterator begin, Iterator end, const Entity& entity) {
	//	return decoder()(begin, end, entity);
	//}

	//template <class decoder, class Entity, class Iterator, class State>
	//int decode(Iterator begin, Iterator end, const Entity& entity, State state) {
	//	return decoder()(begin, end, entity, state);
	//}

}}
