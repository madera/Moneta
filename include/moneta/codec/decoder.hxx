#pragma once
#include "typecode.hxx"
#include "../algorithm/traverse.hxx"
#include "../algorithm/dispatch_entity.hxx"
#include "../traits/fixed_values.hxx"
#include "../make_entity.hxx"

namespace moneta { namespace codec {

	template <class Codec, class Entity, class Enable = void>
	struct entity_decoder;

	template <class Codec, class Path, class Entity, class Enable = void>
	struct enter_entity_decoder {};

	template <class Codec, class Member, class Path, class Enable = void>
	struct member_decoder {
		template <class Entity, class Iterator>
		int operator()(Entity& entity, Iterator begin, Iterator end) const {
			BOOST_MPL_ASSERT_NOT((boost::is_const<typename Member::result_type>));
			return value_decoder<Codec, typename Member::result_type>()(Member()(entity), begin, end);
		}
	};

	template <class Codec, class Member, class Path>
	struct member_decoder<
		Codec, Member, Path,
		typename boost::enable_if<traits::detail::fixed_value<Member> >::type
	> {
		template <class Entity, class Iterator>
		int operator()(Entity& entity, Iterator begin, Iterator end) const {
			BOOST_MPL_ASSERT_NOT((boost::is_const<typename Member::result_type>));
			
			const int result = value_decoder<
				Codec, typename Member::result_type
			>()(Member()(entity), begin, end);

			if (result > 0) {
				if (Member()(entity) != traits::detail::fixed_value<Member>::get()) {
					return 0;
				}
			}

			return result;
		}
	};

	template <class Codec, class T, class Enable = void>
	struct value_decoder;

	template <class Codec, class Path, class Entity, class Enable = void>
	struct leave_entity_decoder {};

	//

	namespace detail {

		template <class Iterator>
		struct decoder_state {
			typedef Iterator iterator_type;

			bool good;
			size_t total_written;

			Iterator& begin; // XXX
			Iterator& end; // XXX

			decoder_state(Iterator& begin_, Iterator& end_)
			 : begin(begin_), end(end_), total_written(0), good(true) {}
		};

		//

		template <class Codec, class Path, class Entity, class State>
		typename boost::enable_if<
			traits::detail::is_functor_callable<
			enter_entity_decoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_enter_entity_decoder(Entity& entity, State& state) {
			return enter_entity_decoder<Codec, Path, Entity>()(entity, state.begin, state.end);
		}

		template <class Codec, class Path, class Entity, class State>
		typename boost::disable_if<
			traits::detail::is_functor_callable<
			enter_entity_decoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_enter_entity_decoder(Entity& entity, State& state) {
			return 0;
		}

		template <class Codec, class Path, class Entity, class State>
		typename boost::enable_if<
			traits::detail::is_functor_callable<
			leave_entity_decoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_leave_entity_decoder(Entity& entity, State& state) {
			return leave_entity_decoder<Codec, Path, Entity>()(entity, state.begin, state.end);
		}

		template <class Codec, class Path, class Entity, class State>
		typename boost::disable_if<
			traits::detail::is_functor_callable<
			leave_entity_decoder<Codec, Path, Entity>,
			void(Entity, typename State::iterator_type, typename State::iterator_type)
			>,
			int
		>::type
		attempt_leave_entity_decoder(Entity& entity, State& state) {
			return 0;
		}

		namespace decoder_handlers {
			template <class Codec>
			struct enter : moneta::algorithm::traverse_enter {
				template <class Path, class Entity, class State>
				void operator()(Entity& entity, State& state) const {
					if (state.good) {
						int result = attempt_enter_entity_decoder<Codec, Path>(entity, state);
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
						int result = member_decoder<Codec, Member, Path>()(
							entity, state.begin, state.end
						);

						if (result > 0) {
							state.begin += result;
							state.total_written += result;
						} else if (result == 0) {
							state.good = false;
							state.total_written = 0;
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
						int result = attempt_leave_entity_decoder<Codec, Path>(entity, state);
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

	template <class Codec, class Entity, class Enable>
	struct entity_decoder {
		template <class Entity, class Iterator>
		int operator()(Entity& entity, Iterator begin, Iterator end) const {
			detail::decoder_state<Iterator> state(begin, end);

			moneta::algorithm::traverse<
				boost::mpl::vector3<
					detail::decoder_handlers::enter<Codec>,
					detail::decoder_handlers::member<Codec>,
					detail::decoder_handlers::leave<Codec>
				>
			>(entity, state);

			return state.total_written;
		}
	};

	template <class Codec, class Entity, class Iterator>
	int decode(Entity& entity, Iterator begin, Iterator end) {
		return entity_decoder<Codec, Entity>()(entity, begin, end);
	}

}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Decode Unknown                                                                                                  ////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "typecode.hxx"

namespace moneta { namespace codec {

	namespace detail {

		template <class Codec, class Visitor, class Iterator>
		struct decode_and_dispatch {
			size_t result;
			
			Visitor& _visitor;
			const Iterator _begin;
			const Iterator _end;

			decode_and_dispatch(Visitor& visitor, Iterator begin, Iterator end)
			 : result(0), _visitor(visitor), _begin(begin), _end(end) {}

			template <class Entity>
			int operator()() {
				Entity entity = moneta::make_entity<Entity>();
				result = moneta::codec::decode<Codec>(entity, _begin, _end);
				if (result > 0) {
					_visitor(entity);
				}

				return result;
			}

		};

		template <class Codec>
		struct entity_type_is {
			typedef typename moneta::codec::detail::typecode_type<Codec>::type typecode_type;
			typecode_type _code;
	
			entity_type_is(const typecode_type& code)
			 : _code(code) {}

			template <class Entity>
			bool operator()() const {
				return _code == moneta::codec::detail::typecode<Codec, Entity>::get();
			}
		};

	}

	template <class Codec, class Entities>	
	struct deducing_entity_decoder {

		template <class Visitor, class Iterator>
		int operator()(Visitor& visitor, Iterator begin, Iterator end) const {
			typename codec::detail::typecode_type<Codec>::type typecode;
			const int result = read_typecode<Codec>(typecode, begin, end);
			if (result <= 0) {
				return result;
			}
			
			return moneta::algorithm::dispatch_entity<Entities>(
				detail::decode_and_dispatch<Codec, Visitor, Iterator>(visitor, begin, end),
				detail::entity_type_is<Codec>(typecode)
			).result;
		}
		
	};

	template <class Codec, class Entities, class Visitor, class Iterator>
	int decode_unknown(Visitor& visitor, Iterator begin, Iterator end) {
		return deducing_entity_decoder<Codec, Entities>()(visitor, begin, end);
	}

	template <class Codec, class Entities, class Visitor, class Iterator>
	int decode_unknowns(Visitor& visitor, Iterator begin, Iterator end) {
		Iterator begin_ = begin;
		Iterator end_ = end;

		int total_consumed = 0;

		int result;
		do {
			result = deducing_entity_decoder<Codec, Entities>()(visitor, begin_, end_);
			if (result > 0) {
				begin_ += result;
				total_consumed += result;
			}
		} while (result > 0);

		return total_consumed;
	}

}}
