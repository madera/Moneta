#pragma once
#include "decoder.hxx"
#include "prefix.hxx"
#include "../limits/member.hxx"
#include "../traits/entity_group.hxx"
#include "../make_entity.hxx"
#include "../algorithm/dispatch_entity.hxx"
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/enum_params_with_a_default.hpp>

#include <boost/mpl/print.hpp> // XXX

namespace moneta { namespace codec {
	
	namespace detail {

		struct no_prefix_reader {};

		template <class Decoder>
		struct daisy_decoder {

			template <class Iterator, class Variant>
			struct looper {

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

				looper(state& state)
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

			template <class Iterator, class Variant>
			int operator()(Iterator begin, Iterator end, Variant& entity) const {
				typedef looper<Iterator, Variant> decoder_type;
				typename decoder_type::state state(begin, end, entity);
				boost::mpl::for_each<typename Variant::types>(decoder_type(state));
				return state.total_read;
			}
		};

		template <class Decoder, class PrefixReader>
		struct prefixed_decoder {

			template <class Variant, class Iterator>
			struct decode_and_dispatch {			
				const Iterator _begin;
				const Iterator _end;
				Variant& _variant;
				size_t& _size;

				decode_and_dispatch(Iterator begin, Iterator end, Variant& variant, size_t& size)
				 : _begin(begin), _end(end), _variant(variant), _size(size) {
					_size = 0;
				}

				template <class Entity>
				int operator()(Entity*) const {
					Entity entity = moneta::make_entity<Entity>();
					_size = Decoder()(_begin, _end, entity);
					if (_size > 0) {
						_variant = entity;
					}

					return _size;
				}

			};

			struct entity_type_is {
				typedef typename PrefixReader::type prefix_type;
				prefix_type _code;
	
				entity_type_is(const prefix_type& code)
				 : _code(code) {}

				template <class Entity>
				bool operator()(const Entity*) const {
					return _code == moneta::codec::detail::prefix_value<PrefixReader, Entity>::get();
				}
			};

			template <class Iterator, class Variant>
			int operator()(Iterator begin, Iterator end, Variant& entity) const {
				typename PrefixReader::type prefix;
				const int result = PrefixReader()(begin, end, prefix);
				if (result <= 0) {
					return result;
				}
			
				size_t size;
				moneta::algorithm::dispatch_entity<typename Variant::types>(
					decode_and_dispatch<Variant, Iterator>(begin, end, entity, size),
					entity_type_is(prefix)
				);
			
				return size;
			}
		};

	}

	template <class Decoder, class KnownEntities, class Prefix = detail::no_prefix_reader>
	struct group_decoder {
		typedef group_decoder this_type;

		typedef Decoder decoder_type;
		typedef KnownEntities known_entities;

		typedef typename boost::make_variant_over<
			typename known_entities::top
		>::type variant_type;

		template <class Iterator, class Prefix_ = Prefix>
		typename boost::enable_if<
			boost::is_same<Prefix_, detail::no_prefix_reader>,
			int
		>::type
		operator()(Iterator begin, Iterator end, variant_type& entity) const {
			return detail::daisy_decoder<Decoder>()(begin, end, entity);
		}

		template <class Iterator, class Prefix_ = Prefix>
		typename boost::disable_if<
			boost::is_same<Prefix_, detail::no_prefix_reader>,
			int
		>::type
		operator()(Iterator begin, Iterator end, variant_type& entity) const {
			return detail::prefixed_decoder<Decoder, Prefix>()(begin, end, entity);
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
