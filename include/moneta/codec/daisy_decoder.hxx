#pragma once
#include "decoder.hxx"
#include "../limits/member.hxx"
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/enum_params_with_a_default.hpp>

namespace moneta { namespace codec {
	
	namespace detail {

		template <class Codec, class Variant, class Iterator>
		struct attempt_decode {

			// XXX: Review these reference members.
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
					const int result = moneta::codec::decode<Codec>(
						_state.begin, _state.end, entity
					);

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
		class T,
		BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_DEC(MONETA_MAX_MEMBERS), class T, boost::mpl::na)
	>
	struct daisy_decoder {
		typedef typename boost::make_variant_over<
			boost::mpl::vector<T, BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(MONETA_MAX_MEMBERS), T)>
		>::type variant_type;

		// XXX: Deprecated
		template <class Iterator>
		int operator()(variant_type& result, Iterator begin, Iterator end) const {
			typedef detail::attempt_decode<Codec, variant_type, Iterator> decoder_type;
			typename decoder_type::state state(result, begin, end);
			boost::mpl::for_each<typename variant_type::types>(decoder_type(state));
			return state.total_read;
		}

		template <class Iterator, class Visitor>
		int operator()(Iterator begin, Iterator end, Visitor& visitor) const {
			variant_type result;
			typedef detail::attempt_decode<Codec, variant_type, Iterator> decoder_type;
			typename decoder_type::state state(result, begin, end);
			boost::mpl::for_each<typename variant_type::types>(decoder_type(state));
			boost::apply_visitor(visitor, result);
			return state.total_read;
		}
	};

}}
