#pragma once
#include "../../traits/entity_name.hxx"
#include "../../traits/member_names.hxx"
#include "../_aux/codec_io.hxx"
#include "../encoder_ng.hxx"

namespace moneta { namespace codec { namespace shell_implementation {

	struct enter_entity_encoder {
		template <class Iterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::empty<Path>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< moneta::traits::get_entity_name<Entity>() << "={"
			;
		}

		template <class Iterator, class Entity, class Path>
		typename boost::disable_if<
			boost::mpl::empty<Path>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Entity& entity, const Path&) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< ' ' << moneta::traits::get_entity_name<Entity>() << "={"
			;
		}
	};

	struct member_encoder {
		template <class Iterator, class Member, class Entity>
		typename boost::disable_if<
			boost::is_same<typename Member::result_type, std::string>, int
		>::type
		operator()(Iterator begin, Iterator end, const Member&, const Entity& entity) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< ' ' << traits::detail::member_name<Member>::get() << '=' << Member()(entity)
			;
		}

		template <class Iterator, class Member, class Entity>
		typename boost::enable_if<
			boost::is_same<typename Member::result_type, std::string>, int
		>::type
		operator()(Iterator begin, Iterator end, const Member&, const Entity& entity) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< ' ' << traits::detail::member_name<Member>::get() << "=\"" << Member()(entity) << '"'
			;
		}
	};

	struct leave_entity_encoder {
		template <class Iterator, class Entity>
		int operator()(Iterator begin, Iterator end, const Entity& entity) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
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
