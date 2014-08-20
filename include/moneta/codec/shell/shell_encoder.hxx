#pragma once
#include "../encoder.hxx"
//#include "traits/xml_traits.hxx"

template <int Size> struct tabs;
template <> struct tabs<0> { static char* get() { return ""; } };
template <> struct tabs<1> { static char* get() { return "\t"; } };
template <> struct tabs<2> { static char* get() { return "\t\t"; } };
template <> struct tabs<3> { static char* get() { return "\t\t\t"; } };
template <> struct tabs<4> { static char* get() { return "\t\t\t\t"; } };

namespace moneta { namespace codec {

 	struct shell;

	template <class Path, class Entity>
	struct enter_entity<shell, Path, Entity> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			std::cerr << moneta::traits::get_entity_name<Entity>() << "={";
			return 0;
		}
	};

	template <class Member, class Path>
	struct member_encoder<shell, Member, Path> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			std::cerr << ' ' << traits::detail::member_name<Member>::get() << "=\"" << Member()(entity) << "\"";
			return 1;
		}
	};

	//

	template <class Path, class Entity>
	struct leave_entity<shell, Path, Entity> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator& begin, Iterator& end) const {
			std::cerr << '}' << std::endl;
			return 0;
		}
	};

}}
