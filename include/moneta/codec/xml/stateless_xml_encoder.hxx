#pragma once
#include "traits/xml_traits.hxx"
#include "../encoder_ng.hxx"
#include "../_aux/path_tabs.hxx"
#include "../codec_io.hxx"
#include "../../traits/entity_name.hxx"
#include "../../traits/member_names.hxx"

namespace moneta { namespace codec { namespace stateless_xml_encoder_implementation {

	namespace detail {
		// XXX: This is ugly. Get these namespaces' s**t together.
		using moneta::codec::detail::is_xml_attribute;
		using moneta::codec::detail::is_xml_element;
		using moneta::codec::detail::has_xml_attributes;
		using moneta::codec::detail::has_xml_elements;
		using moneta::codec::detail::xml_attribute_members;

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
				typename encode_attribute_kv<Entity, Iterator>::state state(_entity, begin, end);

				boost::mpl::for_each<
					typename detail::xml_attribute_members<Entity>::type
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
				boost::mpl::not_<detail::has_xml_attributes<Entity> >,
				boost::mpl::not_<detail::has_xml_elements  <Entity> >
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << traits::get_entity_name<Entity>() << " />\n"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				boost::mpl::not_<detail::has_xml_attributes<Entity> >,
				detail::has_xml_elements<Entity>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << moneta::traits::get_entity_name<Entity>() << ">\n"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				detail::has_xml_attributes<Entity>,
				boost::mpl::not_<detail::has_xml_elements<Entity> >
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << moneta::traits::get_entity_name<Entity>()
				<< detail::attribute_encoder<Entity>(entity)
				<< " />\n"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::and_<
				detail::has_xml_attributes<Entity>,
				detail::has_xml_elements<Entity>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< '<' << moneta::traits::get_entity_name<Entity>()
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
					boost::mpl::not_<detail::has_xml_attributes<Entity> >,
					boost::mpl::not_<detail::has_xml_elements<Entity> >
				>,
				boost::mpl::and_<
					detail::has_xml_attributes<Entity>,
					boost::mpl::not_<detail::has_xml_elements<Entity> >
				>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return 0;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::or_<
				boost::mpl::and_<
					boost::mpl::not_<detail::has_xml_attributes<Entity> >,
					detail::has_xml_elements<Entity>
				>,
				boost::mpl::and_<
					detail::has_xml_attributes<Entity>,
					detail::has_xml_elements<Entity>
				>
			>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path>()
				<< "</" << moneta::traits::get_entity_name<Entity>() << ">\n"
			;
		}
	};

	struct member_encoder {
		template <class Iterator, class Member, class Entity, class Path>
		typename boost::enable_if<
			detail::is_xml_attribute<Member>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Member&, const Entity& entity, const Path&) const {
			// Nothing to do here. Attributes have been taken care of already.
			return  0;
		}

		template <class Iterator, class Member, class Entity, class Path>
		typename boost::enable_if<
			detail::is_xml_element<Member>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Member&, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path, 1>()
				<< '<' << traits::detail::member_name<Member>::get() << '>'
				<< Member()(entity)
				<< "</" << traits::detail::member_name<Member>::get() << '>'
				<< '\n'
			;
		}
	};

	struct enter_container_encoder {
		template <class Iterator, class Member, class Entity, class Path, class State>
		int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path, -1>()
				<< '<' << traits::detail::member_name<Member>::get() << '>' << '\n';
			;
		}
	};

	struct container_member_encoder {
		template <class Iterator, class Member, class Entity, class Path, class State>
		int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
			using moneta::codec::detail::ostringstream;
			ostringstream<Iterator> output = moneta::codec::detail::make_ostringstream(begin, end);
			
			const std::string& tag_name = traits::detail::xml_container_member_element_name<Member>::get();

			//
			// TODO: Replace this code using Spirit's container_iterator or something.
			//
			typedef typename boost::mpl::if_<
				boost::is_const<Entity>,
				typename boost::add_const<typename Member::result_type>::type,
				typename Member::result_type
			>::type container_type;

			typedef typename boost::mpl::if_<
				boost::is_const<Entity>,
				typename container_type::const_iterator,
				typename container_type::iterator
			>::type iterator_type;

			container_type& container = Member()(entity);
			iterator_type itr = container.begin();
			for ( ; itr != container.end(); ++itr) {
				output << aux::path_tabs<Path>()
				       << '<' << tag_name << '>' << *itr << '<' << '/' << tag_name << '>' << '\n';
			}

			return output;
		}
	};

	struct leave_container_encoder {
		template <class Iterator, class Member, class Entity, class Path, class State>
		int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< aux::path_tabs<Path, -1>()
				<< '<' << '/' << traits::detail::member_name<Member>::get() << '>' << '\n';
			;
		}
	};
	
	typedef moneta::codec::encoder<
		enter_actions <enter_entity_encoder>,
		member_actions<member_encoder      >,
		leave_actions <leave_entity_encoder>,
		enter_container_actions <enter_container_encoder >,
		container_member_actions<container_member_encoder>,
		leave_container_actions <leave_container_encoder >
	> stateless_xml_encoder;

}}}

namespace moneta { namespace codec {
	using stateless_xml_encoder_implementation::stateless_xml_encoder;
}}
