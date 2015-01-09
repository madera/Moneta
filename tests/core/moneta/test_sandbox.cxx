#include "stdafx.h"
#include <moneta/container/meta_set.hxx>
#include <moneta/container/data_trackers/pk_tracker.hxx>
#include <moneta/container/data_trackers/rtuple_data.hxx>
#include <moneta/container/load_trackers/bitset_load_tracker.hxx>
#include "model/Person.hxx"

// TODO: Optimize operator generation.
// TODO: Make a user friendly version that requires less typing.
//       Something like: member_comparer<Person, std::string, &Person::Name>
template <class Member>
struct member_comparer {
	typename Member::result_type _value;
public:
	member_comparer(const typename Member::result_type& value)
	 : _value(value) {}

	bool operator==(const typename Member::class_type& entity) const {
		return _value == Member()(entity);
	}

	bool operator!=(const typename Member::class_type& entity) const {
		return !(entity == *this);
	}

	bool operator<(const typename Member::class_type& entity) const {
		return _value < Member()(entity);
	}
};

template <class Member>
struct member_equals : member_comparer<Member> {
	member_equals(const typename Member::result_type& value)
	 : member_comparer<Member>(value) {}

	bool operator()(const typename Member::class_type& entity) const {
		return *this == entity;
	}
};

template <class ForwardIterator, class Predicate, class Operation>
Operation for_each_if(ForwardIterator begin, ForwardIterator end, Predicate predicate, Operation operation) {
	for (; begin != end; ++begin) {
		if (predicate(*begin)) {
			operation(*begin);
		}
	}

	return operation;
}

//namespace moneta {
//
//	namespace detail {
//
//		template <class Member>
//		struct find_impl {
//			template <class Iterator, class Value>
//			Iterator operator()(Iterator begin, Iterator end, Value value) const {
//				for (; begin != end; ++begin) {
//					if (Member()(*begin) == value) {
//						break;
//					}
//				}
//
//				return begin;
//			}
//		};
//
//		template <>
//		struct find_impl<moneta::traits::member<Person, std::string, &Person::Name> > {
//			template <class Iterator, class Value>
//			Iterator operator()(Iterator begin, Iterator end, Value value) const {
//				return begin;
//			}
//		};
//
//	}
//
//	template <class Member, class Iterator, class Value>
//	Iterator find(Iterator begin, Iterator end, Value value) {
//		return detail::find_impl<Member>()(begin, end, value);
//	}
//
//}


BOOST_AUTO_TEST_CASE(sandbox) {



	typedef moneta::traits::member<Person, std::string, &Person::Name> member_t;

	{
		{
			Person  joe;  joe.Name = "Joe";   joe.Fingers = 9;
			Person john; john.Name = "John"; john.Fingers = 21;

			member_equals<
				moneta::traits::member<Person, std::string, &Person::Name>
			> equals("Joe");

			BOOST_ASSERT(equals(joe));
			BOOST_ASSERT(!equals(john));
		}
		{
			Person  joe;  joe.Name = "Joe";   joe.Fingers = 9;
			Person john; john.Name = "John"; john.Fingers = 21;

			member_comparer<
				moneta::traits::member<Person, int, &Person::Fingers>
			> comparer(18);

			BOOST_ASSERT(comparer < john);
			BOOST_ASSERT(!(comparer < joe));
		}
		{
			Person  joe;  joe.Name = "Joe";   joe.Fingers = 9;
			Person john; john.Name = "John"; john.Fingers = 21;

			std::cerr << "================================================\n";

			std::vector<Person> people;
			people.push_back(john);
			people.push_back(joe);


			using moneta::traits::member;

			//BOOST_ASSERT((
			//moneta::find<
			//	moneta::traits::member<Person, std::string, &Person::Name>
			//>(people.begin(), people.end(), "Joe")
			//)
			//== people.begin() + 1
			//);

			for_each_if(
				people.begin(),
				people.end(),
				member_equals<
					member<Person, std::string, &Person::Name>
				>("Joe"),
				[](const Person& person) {
					std::cerr << person.Name << std::endl;
				}
			);

			std::cerr << "================================================\n";

			auto predicate = member_equals<member<Person, std::string, &Person::Name>>("Joe");
			std::vector<Person>::iterator itr = std::find_if(people.begin(), people.end(), predicate);
			BOOST_ASSERT(itr != people.end());
			BOOST_ASSERT(itr->Name == "Joe");

			std::vector<Person>::iterator itr2 = std::find_if(++itr, people.end(), predicate);
			BOOST_ASSERT(itr2 == people.end());
		}
	}



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
