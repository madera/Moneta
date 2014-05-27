#include "stdafx.h"
#include <moneta/container/meta_set.hxx>
#include <moneta/container/data_trackers/pk_tracker.hxx>
#include <moneta/container/data_trackers/rtuple_data.hxx>
#include <moneta/container/load_trackers/bitset_load_tracker.hxx>
#include "model/Person.hxx"

BOOST_AUTO_TEST_CASE(sandbox) {

	typedef moneta::container::meta_set<
		moneta::container::pk_tracker<Person>,
		moneta::container::bitset_load_tracker2<Person>,
		moneta::container::rtuple_data<Person>
	> master_t;

	master_t meta_set;

	auto r0 = meta_set.find(123);
	auto r1 = meta_set.find(1);

	Person john;
	john.ID = 5;
	john.Name = "John Smith";
	john.Height = 1.80;
	john.Fingers = 10;

	{
		master_t::entry e;
		e.pk = 1;
		e.data = moneta::traits::to_rtuple(john);
		
		meta_set.insert(e);
	}

	auto r2 = meta_set.find(1);

	meta_set.replace(r2->pk, *r2);
	
	
	auto data0 = meta_set.find(0);
	auto data1 = meta_set.find(1);


	auto r3 = meta_set.find(1);
	if (r3) {
		r3->data;
	}

	int zzz = 0;
}
