#pragma once

namespace moneta { namespace traits {

	template <class Member>
	inline size_t member_ordinal() {
		typedef moneta::traits::members<Person>::type members;
		return boost::mpl::distance<
			boost::mpl::begin<members>::type,
			boost::mpl::find_if<
				members,
				boost::is_same<
					boost::mpl::_,
					Member
				>
			>::type
		>::value;
	}

}}
