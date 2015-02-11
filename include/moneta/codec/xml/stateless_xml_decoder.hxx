#pragma once

// XXX: Move to proper header file with proper tests.
#include <cctype>

namespace moneta { namespace codec { namespace io {

	template <class Iterator, class Predicate>
	int consume_while(Iterator begin, Iterator end, const Predicate& predicate) {

		Iterator itr;
		for (itr = begin; itr != end && predicate(*itr); ++itr)
			;

		return std::distance(begin, itr);
	}

	template <class Iterator, class OutputIterator, class Predicate>
	int copy_while(Iterator begin, Iterator end, OutputIterator result, const Predicate& predicate) {

		Iterator itr;
		for (itr = begin; itr != end && predicate(*itr); ++itr) {
			*result++ = *itr;
		}

		return std::distance(begin, itr);
	}

	template <class Iterator, class OutputIterator, class Predicate>
	int copy_while(
		Iterator begin, Iterator end, OutputIterator result_begin, OutputIterator result_end,
		const Predicate& predicate
	) {
		Iterator itr;
		for (itr = begin; itr != end && result_begin != result_end && predicate(*itr); ++itr) {
			*result_begin++ = *itr;
		}

		return std::distance(begin, itr);
	}

	struct is_whitespace {		
		bool operator()(const char x) const {
			return std::isspace(x) != 0;
		}
	};

	template <class Iterator>
	int consume_whitespaces(Iterator begin, Iterator end) {
		return consume_while(begin, end, is_whitespace());
	}

}}}

#include "../group_decoder.hxx"
#include "../../traits/entity_name.hxx"
#include "traits/xml_traits.hxx"
#include "../../lexical/set_value.hxx"

namespace moneta { namespace codec { namespace stateless_xml_decoder_implementation {

	struct is_xml_identifier_char {
		typedef char argument_type;

		bool operator()(const char c) const {
			return std::isalnum(c) || c == '_';
		}
	};

	class is_char {
		const char _expected;
	public:
		typedef char argument_type;

		is_char(const char c)
		 : _expected(c) {
		}

		bool operator()(const char c) const {
			return c == _expected;
		}
	};

	template <class Iterator>
	int read_prefix(Iterator begin, Iterator end, std::string& prefix) {
		const size_t minimum = 3; // The bare minimum: "<a "
		const size_t available = std::distance(begin, end);
		if (available < minimum) {
			return 0 - (minimum - available);
		}

		Iterator itr = begin;
		const int whitespaces = moneta::codec::io::consume_whitespaces(itr, end);
		itr += whitespaces;

		if (*itr++ != '<') {
			return 0;
		}

		int result = moneta::codec::io::copy_while(itr, end, std::back_inserter(prefix), is_xml_identifier_char());
		itr += result;

		// If we've reached the end, we can't prove it's the complete entity name.
		if (itr == end) {
			return -1;
		}

		return std::distance(begin, itr);
	}

	struct xml_prefix_reader {
		typedef std::string type;
	
		template <class Iterator>
		int operator()(Iterator begin, Iterator end, type& prefix) const {
			return read_prefix(begin, end, prefix);
		}
	};

	template <class Iterator, class Entity>
	int process_attribute(Iterator begin, Iterator end, Entity& entity) {
		Iterator itr = begin;
		itr += io::consume_whitespaces(itr, end);

		std::string key;
		std::string value;
		
		itr += io::copy_while(itr, end, std::back_inserter(key), is_xml_identifier_char());
		itr += io::consume_whitespaces(itr, end);
		if (*itr++ != '=') {
			return 0;
		}

		itr += io::consume_whitespaces(itr, end);
		if (*itr == '"') {
			itr += io::copy_while(++itr, end, std::back_inserter(value), std::not1(is_char('"')));
			++itr;
		} else if (*itr == '\'') {
			itr += io::copy_while(++itr, end, std::back_inserter(value), std::not1(is_char('\'')));
			++itr;
		} else {
			return 0;
		}

		moneta::lexical::set_value(entity, key, value);
		return std::distance(begin, itr);
	}

	template <class Iterator, class Entity>
	int process_attributes(Iterator begin, Iterator end, Entity& entity) {
		Iterator itr = begin;
		int consumed = 0;

		bool done = false;
		while (!done) {
			const int result = process_attribute(itr, end, entity);
			if (result < 0) {
				return result;
			} else if (result == 0) {
				done = true;
			} else {
				consumed += result;
				itr += result;
			}
		}

		return consumed;
	}

	struct acme_enter_entity {
		template <class Iterator, class Entity, class Path, class State>
		int operator()(Iterator begin, Iterator end, Entity& entity, const Path&, State&) const {
			const std::string entity_name = traits::get_entity_name<Entity>();

			std::string prefix;
			const int prefix_result = read_prefix(begin, end, prefix);
			if (prefix_result <= 0) {
				return prefix_result;
			}

			if (prefix != entity_name) {
				return 0;
			}

			Iterator itr = begin + prefix_result;
			itr += io::consume_whitespaces(itr, end);

			// Do we have attributes?
			if (*itr != '/' && *itr != '>') {
				itr += process_attributes(itr, end, entity);
			}

			itr += io::consume_whitespaces(itr, end);

			// We still need closure. At least one byte for '>', if not two for "/>".
			if (itr == end) {
				return -1;
			}

			if (*itr == '/') {
				++itr;
				if (itr == end) {
					// Need at least '>'.
					return -1;
				}

				// At this point, / must be followed by > to form />.
				if (*itr++ != '>') {
					return 0;
				}

				return std::distance(begin, itr);
			}
			
			// We still need closure. At least one byte for '>', if not two for "/>".
			//if (itr == end) {
			//	return -1;
			//}

			if (*itr++ != '>') {
				return 0;
			}

			// process_members(begin, end, entity);

			// "</entity>"
			const int close_tag_size = 2 + entity_name.size() + 1;

			itr += io::consume_whitespaces(itr, end);
			if (itr == end) {
				return 0 - close_tag_size;
			}

			if (*itr++ != '<') return 0;
			if (itr == end) return 0 - (close_tag_size - 1);

			if (*itr++ != '/') return 0;
			if (itr == end) return 0 - (close_tag_size - 2);

			std::string closing;
			itr += io::copy_while(itr, end, std::back_inserter(closing), is_xml_identifier_char());
			itr += io::consume_whitespaces(itr, end);

			if (closing.size() < entity_name.size()) {
				return 0 - (entity_name.size() - closing.size() + 1); // 1 = '>'
			} else if (closing != entity_name) {
				return 0;
			}

			if (itr == end) return -1;
			if (*itr++ != '>') return 0;

			return std::distance(begin, itr);
		}
	};

	template <class EntityGroup>
	struct stateless_xml_decoder {
		typedef moneta::codec::group_decoder<
			moneta::codec::decoder<
				moneta::codec::enter_actions<acme_enter_entity>
			>,
			EntityGroup,
			xml_prefix_reader
		> type;
	};

}}}

namespace moneta { namespace codec { namespace detail {
	template <class Entity>
	struct prefix_value<stateless_xml_decoder_implementation::xml_prefix_reader, Entity> {
		static std::string get() {
			// XXX: Use XML element names.
			return moneta::traits::get_entity_name<Entity>();
		}
	};
}}}

namespace moneta { namespace codec {
	using stateless_xml_decoder_implementation::stateless_xml_decoder;
}}
