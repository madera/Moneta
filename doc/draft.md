Object versioning strategies
============================

Currently proposed strategies:

 * versionizer<cloning>
 * versionizer<bits>
 * versionizer<hash>

Example Usage
-------------

### Local Usage

	entity_dao<Person> person_dao;
	person_dao.insert(p);
	person_dao.insert(q);
	person_dao.insert(q);

	assert(person_dao.size() == 3);
	
	//person_dao.begin();
	//person_dao.end();
	
	for (const auto& entity : person_dao) {
		dump << entity;
	}

	person_dao.remove(person_dao.begin());
	
### Database Usage

Having a user table with the following records:

| ID | Name          | FavoriteNumber |
| -- |:-------------:|          -----:|
|  1 | Alice         |             10 |
|  2 | Bob           |             15 |
|  3 | Charlie       |             20 |
|  4 | David         |             25 |
|  5 | Edward        |             30 |

This code plays with the table:
	
	entity_dao<Person> person_dao;
	
	assert(person_dao.total() == 10);
	assert(person_dao.size() == 0);

	// Get record from database
	person_dao.fetch(Person::pk_type(1));

	assert(person_dao.size() == 1);
	
	// Search *current* records using pk.	
	person_dao.find(pk_type(1));		// find() with pk_type #db

	person_dao.find_on_database(pk_type(2));

Sample entity
-------------

Throughout the guide we will be using a Person entity for explanations and demonstrations.

	struct Person {
		std::string Name;
		int FavoriteNumber;
	};

### Hashed versionizer

	Person x;
	
	versionizer<Person> v;
	v.is_dirty(x, &Person::Name);

Creates a hash for every (declared) data member. Members must have a std::hash() defined for them.

The object members are compared with their original hash to determine if they are dirty.

No manual housekeeping is necessary for hashed versionizers, due to their comparative nature.

### Bitmap versionizer (Postponed to later version)

Creates a bitmap having one bit for every (declared) data member.

The dirty bits can be set manually or though overloaded operators, such as operator=().

Example

	Person x;	
	versionizer<Person> v;
	
	assert(v.is_dirty(x, &Person::Name) == false);

	v.set_dirty(x, &Person::Name);

	assert(v.is_dirty(x, &Person::Name) == true;
	
In order to use the special operators do this:

property<T> {
  property<T>(m) {
   master = m;
  }
  
  operator=()
}
  
## Technical Details


