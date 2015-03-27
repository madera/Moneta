// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "../group_decoder.hxx"
#include "../../traits/entity_name.hxx"
#include "traits/xml_traits.hxx"
#include "../../lexical/set_value.hxx"
#include "../../lexical/dispatch_member.hxx"
#include "../_aux/io/copy_while.hxx"
#include "../_aux/io/consume_while.hxx"
#include "../_aux/io/consume_whitespaces.hxx"
#include <stack>

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

	struct is_cdata {
		bool operator()(const char c) const {
			// XXX: C'mon...
			return c != '<';
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
		// Iterator key_begin, Iterator key_end, Iterator value_begin, Iterator value_end
		void operator()(Iterator, Iterator, Iterator, Iterator) const {
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
	int ignore_tag(Iterator begin, Iterator end) {
		tag_reader<> reader;

		std::string tag0;
		int result = reader(begin, end, tag0);
		if (result <= 0 || reader.last.is_singleton) {
			return result;
		}

		std::stack<std::string> stack;
		stack.push(tag0);

		Iterator itr = begin + result;

		bool done = false;
		while (!done) {
			itr += io::consume_while(itr, end, is_cdata());

			std::string tag;
			const int result = reader(itr, end, tag);
			if (result == 0) {
				return result;
			} else if (result < 0) {
				// Calculate all the needed tags minimum.
				int required = 0;
				while (!stack.empty()) {
					required += 2 + stack.top().size() + 1; // </ + name + >
					stack.pop();
				}

				return std::distance(itr, end) - required;
			}
			
			itr += result;
			if (reader.last.is_opening) {
				stack.push(tag);
			} else if (!reader.last.is_singleton) {
				if (stack.top() != tag) {
					return 0;
				}

				if (stack.size() == 1) {
					done = true;
				} else {
					stack.pop();
				}
			}
		}

		return std::distance(begin, itr);
	}

	// XXX: Remove the call to read_prefix() on all functions. Just put tag_reader<> to work.
	//      If this is not corrected, prefix will be called each for every tag. Not good.

	template <class Iterator, class Entity, class Path>
	int read_entity(Iterator begin, Iterator end, Entity& entity, Path);

	template <class Member, class Iterator>
	int read_element_member(Iterator begin, Iterator end, typename Member::class_type& entity) {
		Iterator itr = begin;

		tag_reader<
			attribute_assigner<typename Member::class_type>
		> reader = attribute_assigner<typename Member::class_type>(entity);

		std::string opening_tag;
		int result = reader(itr, end, opening_tag);
		if (result <= 0) {
			return result;
		}

		itr += result;

		const std::string element_name = traits::xml_item_name<Member>::get();
		if (opening_tag != element_name) {
			return 0;
		}

		Iterator data_begin = itr;
		itr += io::consume_while(itr, end, is_cdata());

		Iterator data_end = itr;
		std::string data(data_begin, data_end);

		std::string closing_tag;
		result = reader(itr, end, closing_tag);
		if (result <= 0) {
			return result;
		}

		itr += result;

		if (reader.last.is_opening || reader.last.is_singleton || closing_tag != opening_tag) {
			return 0;
		}

		Member()(entity) = boost::lexical_cast<typename Member::result_type>(data);

		return std::distance(begin, itr);
	}

	template <class Iterator, class Entity, class Path>
	struct member_decoder {
		Iterator& _begin;
		Iterator& _end;
		Entity& _entity;

		member_decoder(Iterator& begin, Iterator& end, Entity& entity)
		 : _begin(begin), _end(end), _entity(entity) {}

		template <class Member>
		typename boost::enable_if<
			traits::is_entity<typename Member::result_type>
		>::type
		operator()() const {
			typedef typename boost::mpl::push_back<Path, Member>::type path_type;
			_begin += read_entity(_begin, _end, Member()(_entity), path_type());
		}

		template <class Member>
		typename boost::disable_if<
			traits::is_entity<typename Member::result_type>
		>::type
		operator()() const {
			_begin += read_element_member<Member>(_begin, _end, _entity);
		}
	};

	template <class Iterator, class Entity, class Path>
	int process_member(Iterator begin, Iterator end, Entity& entity, Path) {
		std::string prefix;
		const int prefix_result = read_prefix(begin, end, prefix);
		if (prefix_result <= 0) {
			return prefix_result;
		}

		Iterator itr = begin;
		moneta::lexical::dispatch_member_array_item<Entity, traits::xml_item_name>(
			prefix, member_decoder<Iterator, Entity, Path>(itr, end, entity)
		);

		return std::distance(begin, itr);
	}

	template <class Iterator, class Entity, class Path>
	int process_members(Iterator begin, Iterator end, Entity& entity, Path) {
		Iterator itr = begin;
		int consumed = 0;

		bool done = false;
		while (!done) {
			const int result = process_member(itr, end, entity, Path());
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
	struct get_element_name_impl {
		template <class Path>
		typename boost::enable_if<boost::mpl::empty<Path>, std::string>::type
		operator()(Path) const {
			return traits::xml_item_name<Entity>::get();
		}

		template <class Path>
		typename boost::disable_if<boost::mpl::empty<Path>, std::string>::type
		operator()(Path) const {
			typedef typename boost::mpl::back<Path>::type node;
			return traits::xml_item_name<node>::get();
		}
	};

	template <class Iterator, class Entity, class Path>
	int read_entity(Iterator begin, Iterator end, Entity& entity, Path) {
		const std::string entity_name = get_element_name_impl<Entity>()(Path());

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

		// Opening tag
		//
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

		{
			// TODO: After optionals remodeling, we need to review this calculation.
			//       All non-optional members must be counted and from them a minimum.
			const size_t minimum = 2 + entity_name.size() + 1; // </ + tag + >
			const size_t available = std::distance(itr, end);
			if (available < minimum) {
				return available - minimum;
			}
		}

		itr += io::consume_whitespaces(itr, end);
		result = process_members(itr, end, entity, Path());
		if (result < 0) {
			return result;
		}

		itr += result;

		// Closing tag
		//
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

	struct stateless_xml_decoder_start_entity {
		template <class Iterator, class Entity, class Path>
		int operator()(Iterator begin, Iterator end, Entity& entity, Path) const {
			return read_entity(begin, end, entity, Path());
		}
	};

	struct xml_prefix_reader {
		typedef std::string type;
	
		template <class Iterator>
		int operator()(Iterator begin, Iterator end, type& prefix) const {
			return read_prefix(begin, end, prefix);
		}
	};

	template <class EntityGroup>
	struct stateless_xml_decoder {
		typedef moneta::codec::group_decoder<
			moneta::codec::decoder<
				moneta::codec::start_actions<stateless_xml_decoder_start_entity>
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
			return traits::xml_item_name<Entity>::get();
		}
	};
}}}

namespace moneta { namespace codec {
	using stateless_xml_decoder_implementation::stateless_xml_decoder;
}}
