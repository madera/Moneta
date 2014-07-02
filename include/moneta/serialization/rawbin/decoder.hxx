#pragma once
#include "rawbin_io.hxx"
#include "../for_each_member.hxx"
#include "../../traits/member_names.hxx"
#include "../../traits/fixed_values.hxx"
#include "../../traits/detail/memptr_hacker.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"
#include "../../pp/detail/has_member_trait.hxx"

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		DEFINE_HAS_MEMBER_TRAIT(get)

		template <class Member, class Enable = void>
		struct read_member {
			template <class IteratorType>
			int operator()(typename Member::result_type& value, IteratorType begin, IteratorType end) const {
				return moneta::serialization::rawbin::read(begin, end, value);
			}
		};

		template <class Member>
		struct read_member<
			Member,
			typename boost::enable_if<
				has_member_get<
					traits::detail::fixed_value<Member>
				>
			>::type
		> {
			template <class IteratorType>
			int operator()(typename Member::result_type& value, IteratorType begin, IteratorType end) const {
				int length = moneta::serialization::rawbin::read(begin, end, value);
				if (length >= 0) {
					if (value != traits::detail::fixed_value<Member>::get()) {
						return 0;
					}
				}

				return length;
			}
		};

		template <class IteratorType>
		struct decoder {
			struct state {
				IteratorType& begin;
				IteratorType& end;
				bool good;
				size_t total;

				state(IteratorType& begin_, IteratorType& end_)
				 : begin(begin_), end(end_), good(true), total(0) {
				}
			};

			state& _state;

			decoder(state& state)
			 : _state(state) {
			}

			template <class EntityType, class Member>
			void operator()(EntityType& entity, Member& member) const {
				if (_state.good) {
					int length = read_member<Member>()(member(entity), _state.begin, _state.end);
					if (length <= 0) {
						_state.total = length;
						_state.good = false;
						return;
					}

					_state.begin += length;
					_state.total += length;

					if (_state.begin > _state.end) {
						_state.total = 0;
						_state.good = false;
						return;
					}
				}
			}
		};

	}

	template <class EntityType, class IteratorType>
	int decode(IteratorType begin, IteratorType end, EntityType& entity) {
		detail::decoder<IteratorType>::state state(begin, end);
		moneta::serialization::for_each_member(entity, detail::decoder<IteratorType>(state));
		return state.total;
	}

}}}
