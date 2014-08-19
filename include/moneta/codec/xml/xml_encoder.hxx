#pragma once
#include "../encoder.hxx"
#include "traits/xml_traits.hxx"

template <int Size> struct tabs;
template <> struct tabs<0> { static char* get() { return ""; } };
template <> struct tabs<1> { static char* get() { return "\t"; } };
template <> struct tabs<2> { static char* get() { return "\t\t"; } };
template <> struct tabs<3> { static char* get() { return "\t\t\t"; } };
template <> struct tabs<4> { static char* get() { return "\t\t\t\t"; } };

template <class Member>
struct get_class {
	typedef typename Member::class_type type;
};

namespace moneta { namespace codec {

 	struct xml;

	namespace detail {
		template <class Entity>
		typename boost::enable_if<has_xml_attribute<Entity> >::type
		close_previous_tag() {
			std::cerr << '>' << std::endl;
		}

		template <class Entity>
		typename boost::disable_if<has_xml_attribute<Entity> >::type
		close_previous_tag() {
		}

		template <class Path>
		typename boost::enable_if<boost::mpl::empty<Path> >::type
		before_enter() {
		}

		template <class Path>
		typename boost::disable_if<boost::mpl::empty<Path> >::type
		before_enter() {
			close_previous_tag<typename get_class<typename boost::mpl::back<Path>::type>::type>();
		}
	}

// 	namespace detail {
// 		template <class Entity>
// 		typename boost::enable_if<has_xml_attribute<Entity> >::type
// 		close_previous_tag() {
// 			std::cerr << '>' << std::endl;
// 		}
// 
// 		template <class Entity>
// 		typename boost::disable_if<has_xml_attribute<Entity> >::type
// 		close_previous_tag() {
// 		}
// 
// 		template <class Path, class Entity>
// 		typename boost::enable_if<boost::mpl::empty<Path> >::type
// 		before_member() {
// 		}
// 
// 		template <class Path, class Entity>
// 		typename boost::disable_if<boost::mpl::empty<Path> >::type
// 		before_member() {
// 			close_previous_tag<typename get_class<typename boost::mpl::back<Path>::type>::type>();
// 		}
// 	}

	template <class Path, class Entity>
	struct enter_entity<xml, Path, Entity, typename boost::enable_if<detail::has_xml_attribute<Entity> >::type > {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			detail::before_enter<Path>();

			std::cerr << tabs<boost::mpl::size<Path>::value>::get()
				  << '<' << moneta::traits::get_entity_name<Entity>();
			return 0;
		}
	};

	template <class Path, class Entity>
	struct enter_entity<xml, Path, Entity, typename boost::disable_if<detail::has_xml_attribute<Entity> >::type > {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			detail::before_enter<Path>();
			
			std::cerr << tabs<boost::mpl::size<Path>::value>::get()
				  << '<' << moneta::traits::get_entity_name<Entity>() << '>' << std::endl;
			return 0;
		}
	};

	//

	template <class Member, class Path>
	struct member_encoder<xml, Member, Path, typename boost::enable_if<traits::detail::xml_attribute<Member> >::type> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			std::cerr << ' ' << traits::detail::member_name<Member>::get()
				  << "=\"" << Member()(entity) << "\"";
			return 1;
		}
	};

	template <class Member, class Path>
	struct member_encoder<xml, Member, Path, typename boost::disable_if<traits::detail::xml_attribute<Member> >::type> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			//detail::before_member<Path, typename Member::class_name>();
			std::cerr << std::endl;

			std::cerr << tabs<boost::mpl::size<Path>::value + 1>::get()
				  << '<' << traits::detail::member_name<Member>::get() << '>'
				  << Member()(entity)
				  << "</" << traits::detail::member_name<Member>::get() << '>';
			return 1;
		}
	};

	//

	template <class Path, class Entity>
	struct leave_entity<xml, Path, Entity, typename boost::enable_if<detail::has_xml_attribute<Entity> >::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			std::cerr << '\n' << tabs<boost::mpl::size<Path>::value>::get()
				  << "</" << moneta::traits::get_entity_name<Entity>() << '>' << std::endl;
			return 0;
		}
	};

	template <class Path, class Entity>
	struct leave_entity<xml, Path, Entity, typename boost::disable_if<detail::has_xml_attribute<Entity> >::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			std::cerr << tabs<boost::mpl::size<Path>::value>::get()
				  << "</" << moneta::traits::get_entity_name<Entity>() << '>' << std::endl;
			return 0;
		}
	};

}}
