#pragma once
#include "../encoder.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec {

 	struct debug_dump;
 
	template <class Member, class Path>
	struct member_encoder<debug_dump, Member, Path> {
		typedef typename Member::class_type entity_type;
		typedef typename Member::result_type value_type;

		template <class Iterator>
		int operator()(const entity_type& entity, Member& member, Iterator& begin, Iterator& end) const {
			std::ostringstream oss;
			oss << boost::mpl::size<Path>::value << ": "
			    << traits::get_entity_name<entity_type>() << '.'
			    << traits::detail::member_name<Member>::get() << ": "
			    << member(entity)
			;

			std::cerr << oss.str() << std::endl;
			return oss.str().size();
		}
	};

}}
