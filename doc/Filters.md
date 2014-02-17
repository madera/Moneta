-- Draft document --

##Dynamic SQL Query composition.

###Runtime

Generates queries only usable for SQL. Generated at runtime.

Example:

	std::vector<Person> result = sql::select(age >= 18 && sex == 'F');

##Generic entity filters

###Compile time

Example:

	typedef moneta::static_filter<
		regex_like<
			Name,
			mpl::string<'.*oh', '.*'>
		>
	> custom_filter;
	
	typedef moneta::filter<ContainerType, custom_filter>::type filtered;

### Runtime

Example: Filter using a member value

	std::vector<Person> result = moneta::filter(all_persons, Age >= 18);

Example: Filter using a regex

	std::vector<Person> result = moneta::filter(all_persons, Name == regex(".*oh.*"));
