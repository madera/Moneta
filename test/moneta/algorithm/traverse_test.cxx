#include "stdafx.h"
#include <moneta/algorithm/traverse.hxx>
#include "../model/Cat.hxx"
#include <iostream>

struct enter_traverse_test {
	template <class Entity>
	void operator()(Entity&) const {
		std::cerr << "e:" << moneta::traits::get_entity_name<Entity>() << std::endl;
	}
};

struct entity_traverse_test {
	template <class Entity>
	void operator()(Entity&) const {
		std::cerr << "custom entity:" << typeid(Entity).name() << std::endl;
	}
};

struct member_traverse_test {
	template <class Entity, class Member, class Path>
	void operator()(Entity& entity) const {
		std::cerr << "m:" << moneta::traits::detail::member_name<Member>::get() << std::endl;
	}
};

struct leave_traverse_test {
	template <class Entity>
	void operator()(Entity&) const {
		std::cerr << "l:" << moneta::traits::get_entity_name<Entity>() << std::endl;
	}
};

struct traits {
	typedef enter_traverse_test enter;
	//typedef entity_traverse_test entity;
	typedef member_traverse_test member;
	typedef leave_traverse_test leave;
};

BOOST_AUTO_TEST_CASE(simple_traverse_test) {
	Cat cat;
	moneta::algorithm::traverse<traits>(cat);
}
