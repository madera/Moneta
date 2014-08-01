#pragma once
#include "../encoder.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec {

 	struct shell;
 
	template <class Member, class Path>
	struct member_encoder<shell, Member, Path> {
		typedef typename Member::class_type entity_type;
		typedef typename Member::result_type value_type;

		template <class FromEntity, class ToEntity, class Member>
		void leave(FromEntity& from, ToEntity& to, Member member) const {
			std::ostringstream oss;
			oss << "From " << moneta::traits::get_entity_name<FromEntity>()
			    << " to "  << moneta::traits::get_entity_name<ToEntity>();

			_output.push_back(oss.str());
		}

		template <class Iterator>
		int operator()(const entity_type& entity, Member& member, Iterator& begin, Iterator& end) const {
			std::cerr << boost::format("%|-20|: ") % traits::detail::member_name<Member>::get();
			//value_encoder<shell, value_type>()(member(entity), 0, 0, Path());
			return 1;
		}
	};

}}
