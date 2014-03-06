#pragma once
#include "detail/soci_xlate_read.hxx"

namespace moneta { namespace serialization { namespace soci {

	template <class EntityType, class OutputIteratorType>
	const size_t soci_list(::soci::session& session, OutputIteratorType& output) {
		typedef traits::sql::db_tuple<EntityType>::type db_tuple_type;
		::soci::rowset<db_tuple_type> rows = (
			session.prepare << sql::query::select_all_from_table<EntityType>()
		);
	
		size_t count = 0;
		for (auto itr = rows.begin(); itr != rows.end(); ++itr) { // TODO: C++03
			EntityType newcomer = detail::soci_xlate_read<EntityType>(session, *itr);

			std::cout << "++ READ "
				  << traits::sql::table_name<traits::pure_type<EntityType>::type>::get()
				  << " PK: " << traits::extract_pk(newcomer) << std::endl;

			*output++ = newcomer;
			++count;
		}

		return count;
	}

}}}
