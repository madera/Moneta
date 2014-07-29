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

#define FIXME_MONETA_MAX_MEMBERS 50

	template <
		class Codec,
		class T,
		BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_DEC(FIXME_MONETA_MAX_MEMBERS), class T, boost::mpl::na)
	>
	struct multi_decoder {
		typedef typename boost::make_variant_over<
			boost::mpl::vector<T, BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(FIXME_MONETA_MAX_MEMBERS), T)>
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
