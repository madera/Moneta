#include "stdafx.h"
#include <moneta/pp/detail/cat_include_name.hxx>

#define S1 (Test)
#define S2 (Test)(Model)
#define S3 (Test)(Model)(Basketball)
#define S4 (Test)(Model)(Basketball)(Ball)
#define S5 (Test)(Model)(Basketball)(Ball)(Molecule)
#define S6 (Test)(Model)(Basketball)(Ball)(Molecule)(Atom)
#define S7 (Test)(Model)(Basketball)(Ball)(Molecule)(Atom)(Nucleus)
#define S8 (Test)(Model)(Basketball)(Ball)(Molecule)(Atom)(Nucleus)(Neutron)
#define S9 (Test)(Model)(Basketball)(Ball)(Molecule)(Atom)(Nucleus)(Neutron)(Quark)

BOOST_AUTO_TEST_CASE(cat_include_name_test) {
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S1)), "Test");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S2)), "Test/Model");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S3)), "Test/Model/Basketball");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S4)), "Test/Model/Basketball/Ball");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S5)), "Test/Model/Basketball/Ball/Molecule");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S6)), "Test/Model/Basketball/Ball/Molecule/Atom");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S7)), "Test/Model/Basketball/Ball/Molecule/Atom/Nucleus");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S8)), "Test/Model/Basketball/Ball/Molecule/Atom/Nucleus/Neutron");
	BOOST_CHECK_EQUAL(BOOST_PP_STRINGIZE(MONETA_PP_CAT_INCLUDE_NAME(S9)), "Test/Model/Basketball/Ball/Molecule/Atom/Nucleus/Neutron/Quark");
}
