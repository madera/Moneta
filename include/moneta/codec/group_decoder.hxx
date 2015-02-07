#pragma once
#include "decoder.hxx"
#include "../limits/member.hxx"
#include "../traits/entity_group.hxx"
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/enum_params_with_a_default.hpp>

namespace moneta { namespace codec {
	
	namespace detail {

		template <class Decoder, class Variant, class Iterator>
		struct decode_attempter {

			struct state {
				Iterator begin;
				Iterator end;
				Variant& entity;

				bool done;
				size_t total_read;

				state(Iterator begin_, Iterator end_, Variant& entity_)
				 : begin(begin_), end(end_), entity(entity_), done(false), total_read(0) {}
			};

			state& _state;

			decode_attempter(state& state)
			 : _state(state) {}

			template <class Entity>
			void operator()(Entity&) const {
				if (!_state.done) {
					Entity entity;
					const int result = Decoder()(_state.begin, _state.end, entity);
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

	template <class Decoder, class KnownEntities/*, class Prefix = void*/>
	struct group_decoder {
		typedef group_decoder this_type;

		typedef Decoder decoder_type;
		typedef KnownEntities known_entities;

		typedef typename boost::make_variant_over<
			typename known_entities::top
		>::type variant_type;

		template <class Iterator>
		int operator()(Iterator begin, Iterator end, variant_type& entity) const {
			typedef detail::decode_attempter<Decoder, variant_type, Iterator> decoder_type;
			typename decoder_type::state state(begin, end, entity);
			boost::mpl::for_each<typename variant_type::types>(decoder_type(state));
			return state.total_read;
		}

		template <class Iterator, class Visitor>
		int operator()(Iterator begin, Iterator end, Visitor& visitor) const {
			variant_type entity;
			const int result = (*this)(begin, end, entity);
			if (result > 0) {
				boost::apply_visitor(visitor, entity);
			}

			return result;
		}
	};

}}
