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
#include "../../lexical/dispatch_member.hxx"

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

		if (*itr == '/') {
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

	template <class Entity>
	struct member_decoder {
		//Entity& _entity;
		//const std::string& _value;

		//member_decoder(Entity& entity, const std::string& value)
		// : _entity(entity), _value(value) {}

		template <class Member>
		typename boost::enable_if<
			traits::is_container<typename Member::result_type>
		>::type
		operator()() const {
			//Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
		}

		template <class Member>
		typename boost::disable_if<
			traits::is_container<typename Member::result_type>
		>::type
		operator()() const {
			//Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
		}
	};

	template <class Entity>
	struct attribute_assigner {
		Entity& _entity;

		attribute_assigner(Entity& entity)
		 : _entity(entity) {
		}

		template <class Iterator>
		int operator()(Iterator key_begin, Iterator key_end, Iterator value_begin, Iterator value_end) const {
			std::string key(key_begin, key_end);
			std::string value(value_begin, value_end);
			lexical::set_value(_entity, key, value);
			return 0;
		}
	};

	struct null_handler {
		template <class Iterator>
		void operator()(Iterator key_begin, Iterator key_end, Iterator value_begin, Iterator value_end) const {
			//std::string key(key_begin, key_end);
			//std::string value(value_begin, value_end);
		}
	};

	template <class AttributeHandler = null_handler>
	class tag_reader {
		template <class Iterator>
		int read_attribute(Iterator begin, Iterator end) {
			Iterator itr = begin;
			itr += io::consume_whitespaces(itr, end);

			std::string key;
			std::string value;

			Iterator key_begin, key_end;
			Iterator value_begin, value_end;

			key_begin = itr;
			itr += io::copy_while(itr, end, std::back_inserter(key), is_xml_identifier_char());
			key_end = itr;

			itr += io::consume_whitespaces(itr, end);
			if (*itr++ != '=') {
				return 0;
			}

			itr += io::consume_whitespaces(itr, end);
			if (itr == end) {
				return -1; // need " or '
			}

			const char separator = *itr;
			if (separator != '"' && separator != '\'') {
				return 0;
			}

			value_begin = ++itr;
			itr += io::copy_while(itr, end, std::back_inserter(value), std::not1(is_char(separator)));

			value_end = itr;
			if (itr == end) {
				return 0;
			} else {
				++itr;
			}

			_attribute_handler(key_begin, key_end, value_begin, value_end);
			return std::distance(begin, itr);
		}

		template <class Iterator>
		int read_attributes(Iterator begin, Iterator end) {
			Iterator itr = begin;
			int consumed = 0;

			bool done = false;
			while (!done) {
				const int result = read_attribute(itr, end);
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

		AttributeHandler _attribute_handler;

	public:
		struct last_status {
 			bool is_opening;
			bool has_attributes;
			bool is_singleton;
		} last;

		tag_reader() {
		}

		tag_reader(const AttributeHandler& handler)
		 : _attribute_handler(handler) {
		}

		template <class Iterator>
		int operator()(Iterator begin, Iterator end, std::string& tag) {
			last.is_opening = true;
			last.has_attributes = false;
			last.is_singleton = false;

			Iterator itr = begin;
			itr += io::consume_whitespaces(itr, end);
		
			const size_t needed = 3; // at least for '<a ' or '<a/'
			const size_t available = std::distance(begin, end);
			if (available < needed) {
				return available - needed;
			}

			if (*itr++ != '<') {
				return  0;
			}
		
			if (*itr == '/') {
				last.is_opening = false;
				++itr;
			}

			itr += io::copy_while(itr, end, std::back_inserter(tag), is_xml_identifier_char());
			itr += io::consume_whitespaces(itr, end);

			if (itr == end) {
				return -1;
			}

			if (*itr == '/') {
				last.is_opening = false;
				last.is_singleton = true;
				if (++itr == end) {
					return -1; // need '>'
				}
			}

			if (*itr == '>') {
				return std::distance(begin, itr) + 1;
			}

			const int attributes = read_attributes(itr, end);
			if (attributes <= 0) {
				return 0;
			} else {
				last.has_attributes = true;
				itr += attributes;
			}

			itr += io::consume_whitespaces(itr, end);
			if (itr == end) {
				return -1; // need at least '>'
			}

			if (*itr == '/') {
				last.is_opening = false;
				last.is_singleton = true;
				if (++itr == end) {
					return -1; // need '>'
				}
			}

			if (*itr++ == '>') {
				return std::distance(begin, itr);
			}

			return 0;
		}

	};

	template <class Iterator>
	int ignore_tag(Iterator begin, Iterator end, const std::string& name) {
		//std::stack<std::string> stack;
		
		Iterator itr = begin;

		return std::distance(begin, itr);
	}

	template <class Iterator, class Entity>
	int process_member(Iterator begin, Iterator end, Entity& entity) {
		std::string prefix;
		const int prefix_result = read_prefix(begin, end, prefix);
		if (prefix_result <= 0) {
			return prefix_result;
		}

		// MEQUEDE: Prefix reader is bugged.

		return 0;
	}

	template <class Iterator, class Entity>
	int process_members(Iterator begin, Iterator end, Entity& entity) {
		Iterator itr = begin;
		int consumed = 0;

		bool done = false;
		while (!done) {
			const int result = process_member(itr, end, entity);
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

	template <class Iterator, class Entity, class Path, class State>
	int read_entity(Iterator begin, Iterator end, Entity& entity, const Path&, State&) {
		const std::string entity_name = traits::get_entity_name<Entity>();

		std::string prefix;
		const int prefix_result = read_prefix(begin, end, prefix);
		if (prefix_result <= 0) {
			return prefix_result;
		}

		if (prefix != entity_name) {
			return 0;
		}

		tag_reader<
			attribute_assigner<Entity>
		> reader = attribute_assigner<Entity>(entity);

		Iterator itr = begin;
		std::string tag;
		int result = reader(itr, end, tag);
		if (result <= 0) {
			return result;
		} else {
			itr += result;
		}

		if (tag != entity_name) {
			return 0;
		}

		if (reader.last.is_singleton) {
			return std::distance(begin, itr);
		}

		std::string closing_tag;
		result = reader(itr, end, closing_tag);
		if (result == 0) {
		} else if (result < 0) {
			const size_t closing_required = 2 + tag.size() + 1; // </ + tag + >
			const size_t available = std::distance(itr, end);
			return available - closing_required;
		} else {
			itr += result;
		}

		if (closing_tag != entity_name) {
			return 0;
		}

		return std::distance(begin, itr);
	}

	struct acme_enter_entity {
		template <class Iterator, class Entity, class Path, class State>
		int operator()(Iterator begin, Iterator end, Entity& entity, const Path&, State&) const {
			return read_entity(begin, end, entity, Path(), State());
		}

		template <class Iterator, class Entity, class Path, class State>
		int operator__(Iterator begin, Iterator end, Entity& entity, const Path&, State&) const {
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
			
			if (*itr++ != '>') {
				return 0;
			}

			process_members(itr, end, entity);

			// "</entity>"
			const int close_tag_size = 2 + entity_name.size() + 1;

			itr += io::consume_whitespaces(itr, end);
			if (itr == end) {
				return 0 - close_tag_size;
			}

			if (*itr++ != '<') return 0; if (itr == end) return 0 - (close_tag_size - 1);
			if (*itr++ != '/') return 0; if (itr == end) return 0 - (close_tag_size - 2);

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
