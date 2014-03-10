#include "stdafx.h"

#define INCLUDES ("includer_test_header.hxx")
#include <moneta/pp/detail/includer.hxx>

static void static_test() {
#ifndef TEST_SUCCESS
	// Error: Our test header "includer_test_header.hxx" failed to be included.
	BOOST_STATIC_ASSERT(boost::false_type);
#endif
}

#include <moneta/traits/members.hxx>
#include <moneta/traits/member.hxx>
#include <moneta/traits/member_names.hxx>
#include <moneta/sql/traits/table_name.hxx>
#include <moneta/sql/traits/field_names.hxx>
#include <moneta/traits/pk.hxx>
#include <boost/mpl/vector.hpp>
#include <string>
#include <moneta/pp/core.hxx>

#define RESULT MONETA_DESCRIBE_SQL_ENTITY( \
	Person, PERSON, \
	((int,         ID,      PERSON_ID,     MONETA_PRIMARY_KEY)) \
	((std::string, Name,    PERSON_NAME                      )) \
	((double,      Height,  PERSON_HEIGHT                    )) \
	((int,         Fingers, PERSON_FINGERS                   )) \
)

#pragma message(BOOST_PP_STRINGIZE((RESULT)))