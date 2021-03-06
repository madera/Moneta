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
#include "_aux/get_encoded_size.hxx"

// TODO: Put in detail, well, the detail impls...

namespace moneta { namespace codec {

	using moneta::algorithm::start_actions;
	using moneta::algorithm::enter_actions;
	using moneta::algorithm::member_actions;
	using moneta::algorithm::present_member_actions;
	using moneta::algorithm::leave_actions;
	using moneta::algorithm::enter_container_actions;
	using moneta::algorithm::container_item_actions;
	using moneta::algorithm::leave_container_actions;
	using moneta::algorithm::finish_actions;

	namespace detail {
		using namespace moneta::algorithm::detail;

		template <class Actions, typename Iterator, class UserState>
		struct encoder_state {
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
				detail::traverse_present_member
			>::type present_member_actions;

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

			//
			//
			//

			Iterator begin;
			Iterator end;

			bool good;
			int total_written;
			int last_result;
			
			UserState& substate;

			encoder_state(Iterator begin_, Iterator end_, UserState& substate_)
			 : begin(begin_), end(end_), good(true), total_written(0), last_result(0), substate(substate_) {
			}
		};
	}

	template <class Entity, class EncoderState, class Path>
	class encoder_start_finish_enter_leave_action {
		typedef typename EncoderState::iterator_type Iterator;
		typedef typename EncoderState::userstate_type UserState;

		const Entity& _entity;
		EncoderState& _state;

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
				int (Iterator, Iterator, const Entity&)
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
				int (Iterator, Iterator, const Entity&, Path)
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
				int (Iterator, Iterator, const Entity&, UserState&, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _state.substate, Path())
			);
		}

	public:
		encoder_start_finish_enter_leave_action(const Entity& entity, EncoderState& encoder_state)
		 : _entity(entity), _state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_state.good) {
				process<Action>();
			}
		}
	};

	template <class Entity, class EncoderState, class Member, class Path>
	class encoder_member_or_container_enter_leave_action {
		typedef typename EncoderState::iterator_type Iterator;
		typedef typename EncoderState::userstate_type UserState;

		const Entity& _entity;
		EncoderState& _state;

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
				int (Iterator, Iterator, const Entity&, Member)
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
				int (Iterator, Iterator, const Entity&, Member, Path)
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
				int (Iterator, Iterator, const Entity&, UserState&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, _state.substate, Member(), Path())
			);
		}

	public:
		encoder_member_or_container_enter_leave_action(const Entity& entity, EncoderState& encoder_state)
		 : _entity(entity), _state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_state.good) {
				process<Action>();
			}
		}
	};

	template <class Entity, class EncoderState, class Member, class Path>
	class encoder_present_member_action {
		typedef typename EncoderState::iterator_type Iterator;
		typedef typename EncoderState::userstate_type UserState;

		typedef typename traits::detail::const_if_const<
			Entity,
			typename traits::optional_value_type<
				typename Member::result_type
			>::type
		>::type value_type;

		const Entity& _entity;
		EncoderState& _state;

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
				int (Iterator, Iterator, const Entity&, value_type&, Member)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, traits::get_optional_value(Member()(_entity)), Member())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, const Entity&, value_type&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, traits::get_optional_value(Member()(_entity)), Member(), Path())
			);
		}

		template <typename Action>
		typename boost::enable_if<
			moneta::traits::detail::is_functor_callable<
				Action,
				int (Iterator, Iterator, const Entity&, value_type&, UserState&, Member, Path)
			>
		>::type
		process() const {
			update_result(
				Action()(_state.begin, _state.end, _entity, traits::get_optional_value(Member()(_entity)), _state.substate, Member(), Path())
			);
		}

	public:
		encoder_present_member_action(const Entity& entity, EncoderState& encoder_state)
		 : _entity(entity), _state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			if (_state.good) {
				process<Action>();
			}
		}
	};

	template <class Entity, class Value, class EncoderState, class Member, class Path>
	class encoder_container_item_action {
		typedef typename EncoderState::iterator_type Iterator;
		typedef typename EncoderState::userstate_type UserState;

		Value& _value;
		const Entity& _entity;
		EncoderState& _state;

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
		encoder_container_item_action(Value& value, const Entity& entity, EncoderState& encoder_state)
		 : _value(value), _entity(entity), _state(encoder_state) {}

		template <typename Action>
		void operator()(Action&) const {
			process<Action>();
		}
	};

	struct encoder_start_encoding {
		template <class Entity, class EncoderState, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Path) const {
			boost::mpl::for_each<typename EncoderState::start_actions>(
				encoder_start_finish_enter_leave_action<
					Entity, EncoderState, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_enter_entity {
		template <class Entity, class EncoderState, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Path) const {
			boost::mpl::for_each<typename EncoderState::enter_actions>(
				encoder_start_finish_enter_leave_action<
					Entity, EncoderState, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_member {
		template <class Entity, class EncoderState, class Member, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Member, Path) const {
			boost::mpl::for_each<typename EncoderState::member_actions>(
				encoder_member_or_container_enter_leave_action<
					Entity, EncoderState, Member, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_present_member {
		template <class Entity, class Value, class EncoderState, class Member, class Path>
		void operator()(const Entity& entity, const Value&, EncoderState& encoder_state, Member, Path) const {
			boost::mpl::for_each<typename EncoderState::present_member_actions>(
				encoder_present_member_action<
					Entity, EncoderState, Member, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_leave_entity {
		template <class Entity, class EncoderState, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Path) const {
			boost::mpl::for_each<typename EncoderState::leave_actions>(
				encoder_start_finish_enter_leave_action<
					Entity, EncoderState, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_enter_container {
		template <class Entity, class EncoderState, class Member, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Member, Path) const {
			boost::mpl::for_each<typename EncoderState::enter_container_actions>(
				encoder_member_or_container_enter_leave_action<
					Entity, EncoderState, Member, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_container_item {
		template <class Entity, typename Value, class EncoderState, class Member, class Path>
		void operator()(const Entity& entity, Value& value, EncoderState& encoder_state, Member, Path) const {
			boost::mpl::for_each<typename EncoderState::container_item_actions>(
				encoder_container_item_action<
					Entity, Value, EncoderState, Member, Path
				>(value, entity, encoder_state)
			);
		}
	};

	struct encoder_leave_container {
		template <class Entity, class EncoderState, class Member, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Member, Path) const {
			boost::mpl::for_each<typename EncoderState::leave_container_actions>(
				encoder_member_or_container_enter_leave_action<
					Entity, EncoderState, Member, Path
				>(entity, encoder_state)
			);
		}
	};

	struct encoder_finish_encoding {
		template <class Entity, class EncoderState, class Path>
		void operator()(const Entity& entity, EncoderState& encoder_state, Path) const {
			boost::mpl::for_each<typename EncoderState::finish_actions>(
				encoder_start_finish_enter_leave_action<
					Entity, EncoderState, Path
				>(entity, encoder_state)
			);
		}
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct encoder {
		typedef encoder this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;

		template <class Iterator, class Entity, class Path, class State>
		int _encode(
			Iterator begin, Iterator end, const Entity& entity,
			Path* = 0, State& state = State()
		) const {
			using namespace moneta::algorithm;

			typedef moneta::algorithm::traverse<
				start_actions<encoder_start_encoding>,
				enter_actions<encoder_enter_entity>,
				member_actions<encoder_member>,
				present_member_actions<encoder_present_member>,
				leave_actions<encoder_leave_entity>,
				enter_container_actions<encoder_enter_container>,
				container_item_actions<encoder_container_item>,
				leave_container_actions<encoder_leave_container>,
				finish_actions<encoder_finish_encoding>
			> traverser;

			detail::encoder_state<mpl_vector, Iterator, State> encoder_state(begin, end, state);
			traverser()(entity, encoder_state);

			return (encoder_state.good)? encoder_state.total_written : encoder_state.last_result;
		}

	public:
		template <class Iterator, class Entity>
		int operator()(Iterator begin, Iterator end, const Entity& entity) {
			const boost::mpl::vector0<>* path = 0;
			const detail::no_state state = detail::no_state();
			return _encode(begin, end, entity, path, state);
		}

		template <class Iterator, class Entity, class State>
		int operator()(Iterator begin, Iterator end, const Entity& entity, State& state) {
			const boost::mpl::vector0<>* path = 0;
			return _encode(begin, end, entity, path, state);
		}
	};

	//
	// Syntax candy
	//
	template <class Encoder, class Iterator, class Entity>
	int encode(Iterator begin, Iterator end, const Entity& entity) {
		return Encoder()(begin, end, entity);
	}

	template <class Encoder, class Iterator, class Entity, class State>
	int encode(Iterator begin, Iterator end, const Entity& entity, State& state) {
		return Encoder()(begin, end, entity, state);
	}

	//
	// More Syntax candy
	//

	// TODO: Add legitimate ostream support! And add a version that has state at the end.
	template <class Encoder, class OStream, class Entity>
	typename boost::enable_if<
		traits::is_entity<Entity>,
		int
	>::type
	encode(OStream& ostream, const Entity& entity) {
		const size_t size = aux::get_encoded_size<Encoder>(entity);
		if (size <= 0) {
			return size;
		}

		char* buffer = new char[size];
		const int result = Encoder()(buffer, buffer + size, entity);
		if (result > 0) {
			ostream.write(buffer, size);
		}
		delete[] buffer;

		return result;
	}

	// TODO: Add legitimate ostream support! And add a version that has state at the end.
	template <class Encoder, class OStream, class Entity, class State>
	typename boost::enable_if<
		traits::is_entity<Entity>,
		int
	>::type
	encode(OStream& ostream, const Entity& entity, State& state = detail::no_state()) {
		const size_t size = aux::get_encoded_size<Encoder>(entity, state);
		if (size <= 0) {
			return size;
		}

		char* buffer = new char[size];
		const int result = Encoder()(buffer, buffer + size, entity, state);
		if (result > 0) {
			ostream.write(buffer, size);
		}
		delete[] buffer;

		return result;
	}

}}
