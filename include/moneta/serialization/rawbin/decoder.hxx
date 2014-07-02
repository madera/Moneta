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
					// Validate fixed value.
					if (value != traits::detail::fixed_value<Member>::get()) {
						return 0;
					}
				}

				return length;
			}
		};

		template <class IteratorType>
		struct decoder {
			struct parameters {
				IteratorType& begin;
				IteratorType& end;
				bool good;
				size_t total;

				parameters(IteratorType& begin_, IteratorType& end_)
				 : begin(begin_), end(end_), good(true), total(0) {
				}
			};

			parameters& _parameters;

			decoder(parameters& parameters)
			 : _parameters(parameters) {
			}

			template <class EntityType, class Member>
			void operator()(EntityType& entity, Member& member) const {
				if (_parameters.good) {
					int length = read_member<Member>()(member(entity), _parameters.begin, _parameters.end);
					if (length <= 0) {
						// Short circuit loop.
						_parameters.total = 0;
						_parameters.good = false;
						return;
					}

					_parameters.begin += length;
					_parameters.total += length;

					BOOST_ASSERT(_parameters.begin <= _parameters.end); // XXX
				}
			}
		};

	}

	template <class EntityType, class IteratorType>
	int decode(IteratorType begin, IteratorType end, EntityType& entity) {
		detail::decoder<IteratorType>::parameters parameters(begin, end);
		moneta::serialization::for_each_member(entity, detail::decoder<IteratorType>(parameters));
		return parameters.total;
	}

}}}
