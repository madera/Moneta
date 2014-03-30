#pragma once
#include "../../sql/generators/select.hxx"
#include "detail/soci_xlate_read.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class EntityType, class OutputIteratorType>
	const size_t soci_list(::soci::session& session, OutputIteratorType& output) {
		typedef sql::traits::rtuple<EntityType>::type rtuple_type;
		::soci::rowset<rtuple_type> rows = (
			session.prepare << sql::generators::select_all_from_table<EntityType>()
		);
	
		size_t count = 0;
		for (::soci::rowset<rtuple_type>::const_iterator itr = rows.begin();
			itr != rows.end(); ++itr) {
			EntityType newcomer = detail::soci_xlate_read<EntityType>(session, *itr);
			*output++ = newcomer;
			++count;
		}

		return count;
	}

}}}
