#pragma once
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>

namespace moneta { namespace codec {
	
	namespace detail {

		template <class Codec, class Variant, class Iterator>
		struct attempt_decode {

			struct state {
				Iterator& begin;
				Iterator& end;
				Variant& entity;

				bool done;
				size_t total_read;

				state(Variant& entity_, Iterator& begin_, Iterator& end_)
				 : entity(entity_), begin(begin_), end(end_), done(false), total_read(0) {}
			};

			state& _state;

			attempt_decode(state& state)
			 : _state(state) {}

			template <class Entity>
			void operator()(Entity&) const {
				if (!_state.done) {
					Entity entity;
					const int result = moneta::codec::decode<Codec>(entity, _state.begin, _state.end);
					if (result > 0) {
						_state.done = true;
						_state.entity = entity;
						_state.total_read = result;
					} else if (result < 0) {
						_state.done = true;
						_state.total_read = result;
					}
				}
			}

		};

	}

	template <
		class Codec,
		class T0,
		class T1 = boost::mpl::na, class T2 = boost::mpl::na, class T3 = boost::mpl::na,
		class T4 = boost::mpl::na, class T5 = boost::mpl::na, class T6 = boost::mpl::na,
		class T7 = boost::mpl::na, class T8 = boost::mpl::na, class T9 = boost::mpl::na
	>
	struct multi_decoder {
		typedef typename boost::make_variant_over<
			boost::mpl::vector<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>
		>::type variant_type;

		template <class Iterator>
		int operator()(variant_type& result, Iterator begin, Iterator end) const {
			typedef detail::attempt_decode<Codec, variant_type, Iterator> decoder_type;
			typename decoder_type::state state(result, begin, end);
			boost::mpl::for_each<typename variant_type::types>(decoder_type(state));
			return state.total_read;
		}
	};

}}
