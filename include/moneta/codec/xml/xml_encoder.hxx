#pragma once
#include "../encoder.hxx"
#include "../codec_io.hxx"
#include "traits/xml_traits.hxx"

namespace moneta { namespace codec {

 	struct xml;

	namespace detail {

		// XXX: This is a serious candidate for a library facility.
		//
		//	Motivation: encoding members should be fairly common.
		//
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
			void operator()(Member&) const {
				if (_state.good) {
					int result = moneta::codec::detail::make_ostringstream(_state.begin, _state.end)
						<< ' ' << traits::detail::member_name<Member>::get()
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
				encode_attribute_kv<Entity, Iterator>::state state(_entity, begin, end);

				boost::mpl::for_each<
					typename detail::xml_attribute_members<Entity>::type
				>(encode_attribute_kv<Entity, Iterator>(state));

				return state.total_written;
			}
		};
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	template <class Path, class Entity>
	struct enter_entity_encoder<xml, Path, Entity, typename boost::enable_if<
		boost::mpl::and_<
			boost::mpl::not_<detail::has_xml_attributes<Entity> >,
			boost::mpl::not_<detail::has_xml_elements<Entity> >
		>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(std::begin(buffer), std::end(buffer))
				<< detail::tabs<boost::mpl::size<Path>::value>::get()
				<< '<' << moneta::traits::get_entity_name<Entity>() << " />\n"
			;
		}
	};

	template <class Path, class Entity>
	struct enter_entity_encoder<xml, Path, Entity, typename boost::enable_if<
		boost::mpl::and_<
			boost::mpl::not_<detail::has_xml_attributes<Entity> >,
			detail::has_xml_elements<Entity>
		>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< detail::tabs<boost::mpl::size<Path>::value>::get()
				<< '<' << moneta::traits::get_entity_name<Entity>() << ">\n"
			;
		}
	};

	template <class Path, class Entity>
	struct enter_entity_encoder<xml, Path, Entity, typename boost::enable_if<
		boost::mpl::and_<
			detail::has_xml_attributes<Entity>,
			boost::mpl::not_<detail::has_xml_elements<Entity> >
		>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< detail::tabs<boost::mpl::size<Path>::value>::get()
				<< '<' << moneta::traits::get_entity_name<Entity>()
				<< detail::attribute_encoder<Entity>(entity)
				<< " />\n"
			;
		}
	};

	template <class Path, class Entity>
	struct enter_entity_encoder<xml, Path, Entity, typename boost::enable_if<
		boost::mpl::and_<
			detail::has_xml_attributes<Entity>,
			detail::has_xml_elements<Entity>
		>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< detail::tabs<boost::mpl::size<Path>::value>::get()
				<< '<' << moneta::traits::get_entity_name<Entity>()
				<< detail::attribute_encoder<Entity>(entity)
				<< ">\n"
			;
		}
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class Member, class Path>
	struct member_encoder<xml, Member, Path, typename boost::enable_if<detail::is_xml_attribute<Member> >::type> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			// Nothing to do here. Attributes have been taken care of already.
			return 0;
		}
	};

	template <class Member, class Path>
	struct member_encoder<xml, Member, Path, typename boost::enable_if<detail::is_xml_element<Member> >::type> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< detail::tabs<boost::mpl::size<Path>::value + 1>::get()
				<< '<' << traits::detail::member_name<Member>::get() << '>'
				<< Member()(entity)
				<< "</" << traits::detail::member_name<Member>::get() << '>'
				<< '\n'
			;
		}
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	template <class Path, class Entity>
	struct leave_entity_encoder<xml, Path, Entity, typename boost::enable_if<
		boost::mpl::or_<
			boost::mpl::and_<
				boost::mpl::not_<detail::has_xml_attributes<Entity> >,
				boost::mpl::not_<detail::has_xml_elements<Entity> >
			>,
			boost::mpl::and_<
				detail::has_xml_attributes<Entity>,
				boost::mpl::not_<detail::has_xml_elements<Entity> >
			>
		>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return 0;
		}
	};

	template <class Path, class Entity>
	struct leave_entity_encoder<xml, Path, Entity, typename boost::enable_if<
		boost::mpl::or_<
			boost::mpl::and_<
				boost::mpl::not_<detail::has_xml_attributes<Entity> >,
				detail::has_xml_elements<Entity>
			>,
			boost::mpl::and_<
				detail::has_xml_attributes<Entity>,
				detail::has_xml_elements<Entity>
			>
		>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< detail::tabs<boost::mpl::size<Path>::value>::get()
				<< "</" << moneta::traits::get_entity_name<Entity>() << ">\n"
			;
		}
	};

}}
