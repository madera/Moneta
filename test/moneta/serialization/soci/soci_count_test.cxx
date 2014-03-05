#include "stdafx.h"
#include <moneta/serialization/soci/soci_count.hxx>

const char* get_db_script() {
	return
""
""
	;
}

struct soci_test_fixture {
	soci::session soci_session;

	soci_test_fixture()
	 : soci_session(soci::sqlite3, "../../../../test/db/test.db") {
	}

	~soci_test_fixture() {
	}
};

BOOST_FIXTURE_TEST_CASE(soci_count_test, soci_test_fixture) {
	size_t count = 0;
	soci_session << "select count(*) from CAT", ::soci::into(count);
	BOOST_CHECK_EQUAL(count, 1);
}
