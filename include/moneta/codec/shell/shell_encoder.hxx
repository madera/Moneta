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
#include "../../traits/entity_name.hxx"
#include "../../traits/member_name.hxx"
#include "../_aux/io/ostringstream.hxx"
#include "../encoder.hxx"

namespace moneta { namespace codec { namespace shell_implementation {

	// XXX: Simplify this.
	struct enter_entity_encoder {
		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::empty<Path>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity&, Path) const {
			return io::make_ostringstream(begin, end)
				<< moneta::traits::get_entity_name<Entity>() << "={"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::disable_if<
			boost::mpl::empty<Path>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity&, Path) const {
			return io::make_ostringstream(begin, end)
				<< ' ' << moneta::traits::get_entity_name<Entity>() << "={"
			;
		}
	};

	struct member_encoder {
		template <class Iterator, class Entity, class Member>
		typename boost::disable_if<
			boost::is_same<typename Member::result_type, std::string>, int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, Member) const {
			return io::make_ostringstream(begin, end)
				<< ' ' << traits::member_name<Member>::get()
				<< '=' << Member()(entity)
			;
		}

		template <class Iterator, class Entity, class Member>
		typename boost::enable_if<
			boost::is_same<typename Member::result_type, std::string>, int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, Member) const {
			return io::make_ostringstream(begin, end)
				<< ' ' << traits::member_name<Member>::get()
				<< "=\"" << Member()(entity) << '"'
			;
		}
	};

	struct leave_entity_encoder {
		template <class Iterator, class Entity>
		int operator()(Iterator begin, Iterator end, const Entity&) const {
			return io::make_ostringstream(begin, end)
				<< " }"
			;
		}
	};

	typedef moneta::codec::encoder<
		enter_actions <enter_entity_encoder>,
		member_actions<member_encoder>,
		leave_actions <leave_entity_encoder>
	> shell_encoder;

}}}

namespace moneta { namespace codec {
	using shell_implementation::shell_encoder;
}}
