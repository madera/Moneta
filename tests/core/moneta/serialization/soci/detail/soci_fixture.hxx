#pragma once
#include <moneta/serialization/soci/detail/soci_includes.hxx>

void create_schema(soci::session& session);
void insert_data(soci::session& session);

struct soci_test_fixture {
	soci::session soci_session;

	soci_test_fixture()
	 : soci_session(soci::sqlite3, ":memory:") {
		create_schema(soci_session);
		insert_data(soci_session);
	}

	~soci_test_fixture() {
	}
};
