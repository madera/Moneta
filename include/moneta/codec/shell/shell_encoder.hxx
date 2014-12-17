#pragma once
#include "../../traits/entity_name.hxx"
#include "../encoder.hxx"
#include "../codec_io.hxx"

namespace moneta { namespace codec {

 	struct shell;

	template <class Path, class Entity>
	struct enter_entity_encoder<shell, Path, Entity, typename boost::enable_if<
		boost::mpl::empty<Path>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< moneta::traits::get_entity_name<Entity>() << "={"
			;
		}
	};

	template <class Path, class Entity>
	struct enter_entity_encoder<shell, Path, Entity, typename boost::disable_if<
		boost::mpl::empty<Path>
	>::type> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< ' ' << moneta::traits::get_entity_name<Entity>() << "={"
			;
		}
	};

	//

	template <class Member, class Path>
	struct member_encoder<shell, Member, Path, typename boost::disable_if<
		boost::is_same<typename Member::result_type, std::string>
	>::type> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< ' ' << traits::detail::member_name<Member>::get() << '=' << Member()(entity)
			;
		}
	};

	template <class Member, class Path>
	struct member_encoder<shell, Member, Path, typename boost::enable_if<
		boost::is_same<typename Member::result_type, std::string>
	>::type> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< ' ' << traits::detail::member_name<Member>::get() << "=\"" << Member()(entity) << '"'
			;
		}
	};

	//

	template <class Path, class Entity>
	struct leave_entity_encoder<shell, Path, Entity> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			return moneta::codec::detail::make_ostringstream(begin, end)
				<< " }"
			;
		}
	};

}}
