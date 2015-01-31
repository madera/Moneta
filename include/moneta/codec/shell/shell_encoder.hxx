#pragma once
#include "../../traits/entity_name.hxx"
#include "../../traits/member_names.hxx"
#include "../_aux/io/ostringstream.hxx"
#include "../encoder.hxx"

namespace moneta { namespace codec { namespace shell_implementation {

	struct enter_entity_encoder {
		template <class OutputIterator, class Entity, class Path>
		typename boost::enable_if<
			boost::mpl::empty<Path>,
			int
		>::type
		operator()(
			OutputIterator& next, OutputIterator end,
			const Entity& entity, const Path&
		) const {
			return io::make_ostringstream(next, end)
				<< moneta::traits::get_entity_name<Entity>() << "={"
			;
		}

		template <class OutputIterator, class Entity, class Path>
		typename boost::disable_if<
			boost::mpl::empty<Path>,
			int
		>::type
		operator()(
			OutputIterator& next, OutputIterator end,
			const Entity& entity, const Path&
		) const {
			return io::make_ostringstream(next, end)
				<< ' ' << moneta::traits::get_entity_name<Entity>() << "={"
			;
		}
	};

	struct member_encoder {
		template <class OutputIterator, class Member, class Entity>
		typename boost::disable_if<
			boost::is_same<typename Member::result_type, std::string>, int
		>::type
		operator()(
			OutputIterator& next, OutputIterator end,
			const Member&, const Entity& entity
		) const {
			return io::make_ostringstream(next, end)
				<< ' ' << traits::detail::member_name<Member>::get()
				<< '=' << Member()(entity)
			;
		}

		template <class OutputIterator, class Member, class Entity>
		typename boost::enable_if<
			boost::is_same<typename Member::result_type, std::string>, int
		>::type
		operator()(
			OutputIterator& next, OutputIterator end,
			const Member&, const Entity& entity
		) const {
			return io::make_ostringstream(next, end)
				<< ' ' << traits::detail::member_name<Member>::get()
				<< "=\"" << Member()(entity) << '"'
			;
		}
	};

	struct leave_entity_encoder {
		template <class OutputIterator, class Entity>
		int operator()(
			OutputIterator& next, OutputIterator end,
			const Entity& entity
		) const {
			return io::make_ostringstream(next, end)
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
