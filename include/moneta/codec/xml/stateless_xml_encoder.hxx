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
#include "traits/xml_traits.hxx"
#include "../encoder.hxx"
#include "../_aux/path_tabs.hxx"
#include "../_aux/io/ostringstream.hxx"
#include "../../traits/entity_name.hxx"
#include "../../traits/member_name.hxx"

namespace moneta { namespace codec { namespace stateless_xml_encoder_implementation {

	namespace detail {

		template <class Entity, class Iterator>
		struct encode_attribute_kv {

			struct state {
				const Entity& entity;

				Iterator begin;
				Iterator end;

				size_t total_written;
				bool good;

				state(const Entity& entity_, Iterator begin_, Iterator end_)
				 : entity(entity_), begin(begin_), end(end_), total_written(0), good(true) {}
			};	

			state& _state;

			encode_attribute_kv(state& state)
			 : _state(state) {}

			template <class Member>
			void operator()(Member) const {
				if (_state.good) {
					int result = io::make_ostringstream(_state.begin, _state.end)
						<< ' ' << traits::xml_item_name<Member>::get()
						<< "=\"" << Member()(_state.entity) << '"'
					;

					if (result > 0) {
						_state.begin += result;
						_state.total_written += result;
					} else  if (result < 0) {
						_state.good = false;
						_state.total_written = result;
					}
				}
			}
		};

		template <class Entity>
		struct attribute_encoder {
			const Entity& _entity;

			attribute_encoder(const Entity& entity)
			 : _entity(entity) {}

			template <class Iterator>
			int operator()(Iterator begin, Iterator end) const {
				typedef typename encode_attribute_kv<
					Entity, Iterator
				>::state state_type;
				
				state_type state(_entity, begin, end);

				boost::mpl::for_each<
					typename traits::xml_attribute_members<Entity>::type
				>(encode_attribute_kv<Entity, Iterator>(state));

				return state.total_written;
			}
		};
	}

	struct enter_entity_encoder {

		//
		// enter
		//
		//	has_xml_attributes	has_xml_elements	output
		//	
		//		0			0		<element />
		//		0			1		<element>
		//		1			0		<element a0="v0" a1="v1" />
		//		1			1		<element a0="v0" a1="v1">
		//

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				boost::mpl::not_<traits::has_xml_attributes<Entity> >,
				boost::mpl::not_<traits::has_xml_elements  <Entity> >
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity&, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << traits::get_xml_item_name<Entity, Path>() << " />\n"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				boost::mpl::not_<traits::has_xml_attributes<Entity> >,
				traits::has_xml_elements<Entity>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity&, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << traits::get_xml_item_name<Entity, Path>() << ">\n"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				traits::has_xml_attributes<Entity>,
				boost::mpl::not_<traits::has_xml_elements<Entity> >
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << traits::get_xml_item_name<Entity, Path>()
				<< detail::attribute_encoder<Entity>(entity)
				<< " />\n"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				traits::has_xml_attributes<Entity>,
				traits::has_xml_elements<Entity>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << traits::get_xml_item_name<Entity, Path>()
				<< detail::attribute_encoder<Entity>(entity)
				<< ">\n"
			;
		}

	};

	struct leave_entity_encoder {

		//
		// leave
		//
		//	has_xml_attributes	has_xml_elements	output
		//	
		//		0			0		(nothing)
		//		0			1		</element>
		//		1			0		(nothing)
		//		1			1		</element>
		//

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::or_<
				boost::mpl::and_<
					boost::mpl::not_<traits::has_xml_attributes<Entity> >,
					boost::mpl::not_<traits::has_xml_elements<Entity> >
				>,
				boost::mpl::and_<
					traits::has_xml_attributes<Entity>,
					boost::mpl::not_<traits::has_xml_elements<Entity> >
				>
			>,
			int
		>::type
		operator()(Iterator, Iterator, const Entity&, Path) const {
			return 0;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::or_<
				boost::mpl::and_<
					boost::mpl::not_<traits::has_xml_attributes<Entity> >,
					traits::has_xml_elements<Entity>
				>,
				boost::mpl::and_<
					traits::has_xml_attributes<Entity>,
					traits::has_xml_elements<Entity>
				>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity&, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< "</" << traits::get_xml_item_name<Entity, Path>() << ">\n"
			;
		}
	};

	struct present_member_encoder {
		template <class Iterator, class Entity, class Value, class Member, class Path>
		typename boost::enable_if<
			traits::is_xml_attribute<Member>,
			int
		>::type
		operator()(Iterator, Iterator, const Entity&, const Value&, Member, Path) const {
			// Nothing to do here. Attributes have been taken care of already.
			return  0;
		}

		template <class Iterator, class Entity, class Value, class Member, class Path>
		typename boost::enable_if<
			traits::is_xml_element<Member>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity&, const Value& value, Member, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path, 1>()
				<< '<' << traits::xml_item_name<Member>::get() << '>'
				<< value
				<< "</" << traits::xml_item_name<Member>::get() << '>'
				<< '\n'
			;
		}
	};

	struct enter_container_encoder {
		template <class Iterator, class Entity, class Member, class Path>
		int operator()(Iterator begin, Iterator end, const Entity&, Member, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path, -1>()
				<< '<' << traits::xml_item_name<Member>::get() << '>' << '\n';
			;
		}
	};

	struct container_item_encoder {
		template <class Iterator, class Entity, class Value, class Member, class Path>
		int operator()(Iterator begin, Iterator end, const Entity&, const Value& value, Member, Path) const {
			const std::string& tag_name = traits::xml_container_item_name<Member>::get();
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << tag_name << '>' << value << '<' << '/' << tag_name << '>' << '\n'
			;
		}
	};

	struct leave_container_encoder {
		template <class Iterator, class Entity, class Member, class Path>
		int operator()(Iterator begin, Iterator end, const Entity&, Member, Path) const {
			return io::make_ostringstream(begin, end)
				<< aux::path_tabs<Path, -1>()
				<< '<' << '/' << traits::xml_item_name<Member>::get() << '>' << '\n';
			;
		}
	};
	
	typedef moneta::codec::encoder<
		enter_actions<enter_entity_encoder>,
		present_member_actions<present_member_encoder>,
		leave_actions<leave_entity_encoder>,
		enter_container_actions<enter_container_encoder >,
		container_item_actions<container_item_encoder>,
		leave_container_actions<leave_container_encoder>
	> stateless_xml_encoder;

}}}

namespace moneta { namespace codec {
	using stateless_xml_encoder_implementation::stateless_xml_encoder;
}}
