// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "../algorithm/traverse.hxx"
#include "../traits/fixed_values.hxx"

// TODO: Put in detail, well, the detail impls...

namespace moneta { namespace codec {

	using moneta::algorithm::start_actions;
	using moneta::algorithm::enter_actions;
	using moneta::algorithm::member_actions;
	using moneta::algorithm::leave_actions;
	using moneta::algorithm::enter_container_actions;
	using moneta::algorithm::container_item_actions;
	using moneta::algorithm::leave_container_actions;
	using moneta::algorithm::finish_actions;

	namespace detail {
		using namespace moneta::algorithm::detail;

		template <class Actions, typename Iterator, class UserState>
		struct decoder_state {
			typedef Actions actions;
			typedef Iterator iterator_type;
			typedef UserState userstate_type;

			//
			//
			//
			typedef typename detail::actions_of<
				Actions,
				detail::traverse_start
			>::type start_actions;

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

			typedef typename detail::actions_of<
				Actions,
				detail::traverse_finish
			>::type finish_actions;

			Iterator begin;
			Iterator end;

			bool good;
			int total_written;
			int last_result;
			
			UserState& substate;

			decoder_state(Iterator begin_, Iterator end_, UserState& substate_)
			 : begin(begin_), end(end_), good(true), total_written(0), last_result(0), substate(substate_) {
			}
		};
	}

	template <class Entity, class DecoderState, class Path>
	class decoder_start_finish_enter_leave_action {
		typedef typename DecoderState::iterator_type Iterator;
		typedef typename DecoderState::userstate_type UserState;

		Entity& _entity;
		DecoderState& _state;

		void update_result(const int result) const {
			if (result < 0) {
				_state.good = false;
			} else if (result > 0) {
				_state.begin += result;
				_state.total_written += result;
			} else {
			}

			_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity)
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, Path())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, UserState&, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _state.substate, Path())
			);
		}

	public:
		decoder_start_finish_enter_leave_action(Entity& entity, DecoderState& decoder_state)
		 : _entity(entity), _state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_state.good) {
				process<Action>();
			}
		}
	};

	template <class Entity, class DecoderState, class Member, class Path>
	class decoder_member_or_container_enter_leave_action {
		typedef typename DecoderState::iterator_type Iterator;
		typedef typename DecoderState::userstate_type UserState;

		Entity& _entity;
		DecoderState& _state;

		void update_result(const int result) const {
			if (result < 0) {
				_state.good = false;
			} else if (result > 0) {
				_state.begin += result;
				_state.total_written += result;
			} else {
			}

			_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, Member)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, Member())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, Member(), Path())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, UserState&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _state.substate, Member(), Path())
			);
		}
			
	public:
		decoder_member_or_container_enter_leave_action(Entity& entity, DecoderState& decoder_state)
		 : _entity(entity), _state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_state.good) {
				process<Action>();
			}
		}
	};

	template <class Entity, class Value, class DecoderState, class Member, class Path>
	class decoder_container_item_action {
		typedef typename DecoderState::iterator_type Iterator;
		typedef typename DecoderState::userstate_type UserState;

		Value& _value;
		Entity& _entity;
		DecoderState& _state;

		void update_result(const int result) const {
			if (result < 0) {
				_state.good = false;
			} else if (result > 0) {
				_state.begin += result;
				_state.total_written += result;
			} else {
			}

			_state.last_result = result;
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, Value&, Member)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _value, Member())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, Value&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _value, Member(), Path())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, Entity&, Value&, UserState&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _value, _state.substate, Member(), Path())
			);
		}

	public:
		decoder_container_item_action(Value& value, Entity& entity, DecoderState& decoder_state)
		 : _value(value), _entity(entity), _state(decoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};
	
	struct decoder_start_decoding {
		template <class Entity, class DecoderState, class Path>
		void operator()(Entity& entity, DecoderState& decoder_state, Path) const {
			boost::mpl::for_each<typename DecoderState::start_actions>(
				decoder_start_finish_enter_leave_action<
					Entity, DecoderState, Path
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_enter_entity {
		template <class Entity, class DecoderState, class Path>
		void operator()(Entity& entity, DecoderState& decoder_state, Path) const {
			boost::mpl::for_each<typename DecoderState::enter_actions>(
				decoder_start_finish_enter_leave_action<
					Entity, DecoderState, Path
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_member {
		template <class Entity, class DecoderState, class Member, class Path>
		typename boost::enable_if<
			traits::fixed_value<Member>
		>::type
		operator()(Entity& entity, DecoderState& decoder_state, Member, Path) const {
			boost::mpl::for_each<typename DecoderState::member_actions>(
				decoder_member_or_container_enter_leave_action<
					Entity, DecoderState, Member, Path
				>(entity, decoder_state)
			);

			if (decoder_state.good) {
				if (Member()(entity) != traits::fixed_value<Member>::get()) {
					decoder_state.good = false;
					decoder_state.total_written = 0; // XXX: Confirm this is needed.
					decoder_state.last_result = 0;
				}
			}
		}

		template <class Entity, class DecoderState, class Member, class Path>
		typename boost::disable_if<
			traits::fixed_value<Member>
		>::type
		operator()(Entity& entity, DecoderState& decoder_state, Member, Path) const {
			boost::mpl::for_each<typename DecoderState::member_actions>(
				decoder_member_or_container_enter_leave_action<
					Entity, DecoderState, Member, Path
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_leave_entity {
		template <class Entity, class DecoderState, class Path>
		void operator()(Entity& entity, DecoderState& decoder_state, Path) const {
			boost::mpl::for_each<typename DecoderState::leave_actions>(
				decoder_start_finish_enter_leave_action<
					Entity, DecoderState, Path
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_enter_container {
		template <class Entity, class DecoderState, class Member, class Path>
		void operator()(Entity& entity, DecoderState& decoder_state, Member, Path) const {
			boost::mpl::for_each<typename DecoderState::enter_container_actions>(
				decoder_member_or_container_enter_leave_action<
					Entity, DecoderState, Member, Path
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_container_item {
		template <class Entity, typename Value, class DecoderState, class Member, class Path>
		void operator()(Entity& entity, Value& value, DecoderState& decoder_state, Member, Path) const {
			boost::mpl::for_each<typename DecoderState::container_item_actions>(
				decoder_container_item_action<
					Entity, Value, DecoderState, Member, Path
				>(value, entity, decoder_state)
			);
		}
	};

	struct decoder_leave_container {
		template <class Entity, class DecoderState, class Member, class Path>
		void operator()(Entity& entity, DecoderState& decoder_state, Member, Path) const {
			boost::mpl::for_each<typename DecoderState::leave_container_actions>(
				decoder_member_or_container_enter_leave_action<
					Entity, DecoderState, Member, Path
				>(entity, decoder_state)
			);
		}
	};

	struct decoder_finish_decoding {
		template <class Entity, class DecoderState, class Path>
		void operator()(Entity& entity, DecoderState& decoder_state, Path) const {
			boost::mpl::for_each<typename DecoderState::finish_actions>(
				decoder_start_finish_enter_leave_action<
					Entity, DecoderState, Path
				>(entity, decoder_state)
			);
		}
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct decoder {
		typedef decoder this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;

		template <class Iterator, class Entity, class Path, class State>
		int _decode(
			Iterator begin, Iterator end, Entity& entity,
			Path* = 0, State& state = State()
		) const {
			using namespace moneta::algorithm;

			typedef moneta::algorithm::traverse<
				start_actions<decoder_start_decoding>,
				enter_actions<decoder_enter_entity>,
				member_actions<decoder_member>,
				leave_actions<decoder_leave_entity>,
				enter_container_actions<decoder_enter_container>,
				container_item_actions<decoder_container_item>,
				leave_container_actions<decoder_leave_container>,
				finish_actions<decoder_finish_decoding>
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
	// Syntax candy
	//
	template <class Decoder, class Iterator, class Entity>
	int decode(Iterator begin, Iterator end, Entity& entity) {
		return Decoder()(begin, end, entity);
	}

	template <class Decoder, class Iterator, class Entity, class State>
	int decode(Iterator begin, Iterator end, Entity& entity, State& state) {
		return Decoder()(begin, end, entity, state);
	}

}}
