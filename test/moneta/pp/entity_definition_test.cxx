#include "stdafx.h"
#define MONETA_ENTITY_PROCESSOR <moneta/pp/specs/struct_spec.hxx>
#include "../model/Ball.hxx"

BOOST_AUTO_TEST_CASE(entity_definition_test) {
	Test::Model::Ball ball;
	ball.ID = 123;
	ball.Description = "Basketball Supreme Original";
	ball.Sport = "Basketball";
}
