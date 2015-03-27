#include "pch.hxx"
#include <moneta/algorithm/dispatch_entity.hxx>
#include <moneta/make_entity.hxx>
#include <sstream>
#include "../model/Person.hxx"
#include "../model/Composite.hxx"
#include "../model/Cat.hxx"

struct test_operation {
	std::ostringstream& _oss;

	test_operation(std::ostringstream& oss)
	 : _oss(oss) {}

	template <class Entity>
	void operator()(Entity*) const {
		_oss << moneta::traits::entity_name<Entity>::get();
	}
};

struct entity_name_is {
	const std::string _name;
	
	entity_name_is(const std::string& name)
	 : _name(name) {}

	template <class Entity>
	bool operator()(Entity*) const {
		return _name == moneta::traits::entity_name<Entity>::get();
	}
};

BOOST_AUTO_TEST_CASE(algorithm_dispatch_entity_test) {
	typedef boost::mpl::vector5<Person, Address, Cat, Composite, Dog> entities;

	std::ostringstream oss;

	moneta::algorithm::dispatch_entity<entities>(test_operation(oss), entity_name_is("Cat"      ));
	moneta::algorithm::dispatch_entity<entities>(test_operation(oss), entity_name_is("Address"  ));
	moneta::algorithm::dispatch_entity<entities>(test_operation(oss), entity_name_is("Person"   ));
	moneta::algorithm::dispatch_entity<entities>(test_operation(oss), entity_name_is("Dog"      ));
	moneta::algorithm::dispatch_entity<entities>(test_operation(oss), entity_name_is("Composite"));

	BOOST_CHECK_EQUAL(oss.str(), "CatAddressPersonDogComposite");
}
