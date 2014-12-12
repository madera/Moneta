#pragma once
#include "../../sql/generators/select.hxx"
#include "detail/soci_xlate_read.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class Entity, class OutputIteratorType>
	const size_t soci_list(::soci::session& session, OutputIteratorType& output) {
		typedef traits::rtuple<Entity>::type rtuple_type;
		::soci::rowset<rtuple_type> rows = (
			session.prepare << sql::generators::select_all_from_table<Entity>()
		);
	
		size_t count = 0;
		for (::soci::rowset<rtuple_type>::const_iterator itr = rows.begin();
			itr != rows.end(); ++itr) {
			Entity newcomer = detail::soci_xlate_read<Entity>(session, *itr);
			*output++ = newcomer;
			++count;
		}

		return count;
	}

}}}
