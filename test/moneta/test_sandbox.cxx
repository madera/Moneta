#include "stdafx.h"
#include <moneta/traits/pk.hxx>
#include <moneta/traits/rtuple.hxx>
#include <moneta/traits/to_rtuple.hxx>
#include <moneta/traits/extract_pk.hxx>
#include <boost/call_traits.hpp>
#include "model/Person.hxx"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/include/io.hpp>

#include <boost/mpl/inherit_linearly.hpp>

#include <boost/mpl/print.hpp>

//--------------------------------------------------------------------------------------------------

#include <moneta/pp/detail/has_member_trait.hxx>
DEFINE_HAS_NESTED_TYPE(get_index)

//--------------------------------------------------------------------------------------------------

#include <moneta/pp/detail/has_member_trait.hxx>
DEFINE_HAS_MEMBER_TRAIT(to_string)

template <class T>
typename boost::enable_if<has_member_to_string<T>, std::string>::type
to_string_or_empty(const T& entity) {
	return entity.to_string();
}

template <class T>
typename boost::disable_if<has_member_to_string<T>, std::string>::type
to_string_or_empty(const T& entity) {
	return "";
}

//--------------------------------------------------------------------------------------------------

#include <bitset>

template <class Master, class EntityType>
struct bitset_load_tracker_impl {
	typedef bitset_load_tracker_impl this_type;

	struct entry {
		typedef typename std::bitset<
			boost::mpl::size<
				typename moneta::traits::rtuple<EntityType>::type
			>::value
		> state_type;

		state_type load_state;

		entry() {
		}

		entry(const EntityType& entity) {
		}

		std::string to_string() const {
			return boost::lexical_cast<std::string>(load_state);
		}

		bool operator<(const state_type& rhs) const {
			return false;
		}
	};

	const bool is_loaded(const int ordinal) const {
		return 1;
	}
};

//--------------------------------------------------------------------------------------------------

template <class Master, class EntityType>
struct pk_tracker_impl {

	typedef pk_tracker_impl this_type;

	typedef typename boost::call_traits<
		typename moneta::traits::pk<EntityType>::type
	>::param_type param_type;

	struct entry {
		typedef typename moneta::traits::pk<EntityType>::type state_type;
		state_type pk;

		entry() {}

		entry(const EntityType& entity) {
			pk = moneta::traits::extract_pk(entity);
		}

		const bool operator==(param_type rhs) const {
			return pk == rhs;
		}

		std::string to_string() const {
			return boost::lexical_cast<std::string>(pk);
		}
	};

	struct get_index {
		template <class MasterEntry>
		struct apply {
			typedef boost::multi_index::hashed_unique<
				boost::multi_index::tag<this_type>,
				boost::multi_index::member<
					entry,
					typename entry::state_type,
					&entry::pk
				>
			> type;
		};
	};

	template <class LazyHack = Master>
	boost::optional<typename LazyHack::master_entry>
	find(param_type pk) const {
		const Master::container_type& _container = Master::container(this);
		typedef typename Master::container_type::template index<this_type>::type index;
		index::const_iterator begin = _container.get<this_type>().begin();
		index::const_iterator end   = _container.get<this_type>().end();
		index::const_iterator itr   = std::find(begin, end, pk);
		return (itr == end)? boost::optional<typename Master::master_entry>() : *itr;
	}

	// DEPRECATED UNTIL REFACTORING
	template <class LazyHack = Master>
	void replace(param_type pk, typename LazyHack::master_entry& entry_) {
		Master::container_type& _container = Master::container(this);
		typedef typename Master::container_type::template index<this_type>::type index;
		index::const_iterator begin = _container.get<this_type>().begin();
		index::const_iterator end   = _container.get<this_type>().end();
		index::const_iterator itr   = std::find(begin, end, pk);
		assert(itr != end);
		_container.get<this_type>().replace(itr, entry_);
	}
};

//--------------------------------------------------------------------------------------------------

template <class Master, class EntityType>
struct rtuple_data_impl {
	typedef rtuple_data_impl this_type;

	struct entry {
		typedef typename moneta::traits::rtuple<EntityType>::type state_type;
		state_type data;

		entry() {}

		entry(const EntityType& entity) {
			data = moneta::traits::to_rtuple(entity);
		}

		const bool operator==(const state_type& rhs) const {
			return data == rhs;
		}

		std::string to_string() const {
			std::ostringstream oss;
			oss << boost::fusion::tuple_open("(")
			    << boost::fusion::tuple_close(")")
			    << boost::fusion::tuple_delimiter(", ")
			    << data;
			
			return oss.str();
		}
	};
};

//--------------------------------------------------------------------------------------------------

template <class InstanceType, class ArgumentType>
class sliced_constructor {
	InstanceType& _instance;
	ArgumentType& _argument;
public:
	sliced_constructor(InstanceType& instance, ArgumentType& argument)
	 : _instance(instance), _argument(argument) {}

	template <class Base>
	void operator()(const Base& ignored) const {
		BOOST_MPL_ASSERT((boost::is_base_and_derived<Base, InstanceType>));
		Base* base_ptr = &_instance;
		new (base_ptr) Base(_argument); // Placement new on sliced entity.
	}
};

template <class InstanceType, class OstreamType = std::basic_ostream<char> >
class sliced_to_string_or_empty_ostreamer {
	const InstanceType& _instance;
	OstreamType& _output;
	std::string _separator;
	const size_t _size;
	size_t _iteration;
public:
	sliced_to_string_or_empty_ostreamer(
		const InstanceType& instance,
		OstreamType& output,
		const std::string separator = ", ",
		const size_t size = -1
	) : _instance(instance), _output(output), _separator(separator), _size(size), _iteration(0) {}

	template <class Base>
	void operator()(const Base& ignored) {
		BOOST_MPL_ASSERT((boost::is_base_and_derived<Base, InstanceType>));
		const Base& base = _instance;
		//_output << typeid(Base).name();
		_output << to_string_or_empty(base);

		if (_size == -1 || ++_iteration < _size) {
			_output << _separator;
		}
	}

	const OstreamType& get_ostream() const {
		return _output;
	}
};

//--------------------------------------------------------------------------------------------------

template <
	class T0,
	class T1,
	class T2
>
struct master :
	boost::mpl::apply<T0, master<T0, T1, T2> >::type,
	boost::mpl::apply<T1, master<T0, T1, T2> >::type,
	boost::mpl::apply<T2, master<T0, T1, T2> >::type
{
	typedef master this_type;

	typedef typename boost::mpl::apply<T0, this_type>::type t0_type;
	typedef typename boost::mpl::apply<T1, this_type>::type t1_type;
	typedef typename boost::mpl::apply<T2, this_type>::type t2_type;

	typedef boost::mpl::vector<t0_type, t1_type, t2_type> traits_type;

	template <class T>
	struct get_entry {
		typedef typename T::entry type;
	};

	// Vector of trait's entries.
	typedef typename boost::mpl::fold<
		traits_type,
		boost::mpl::vector0<>,
		boost::mpl::push_back<
			boost::mpl::_1,
			get_entry<boost::mpl::_2>
		>
	>::type entries_type;

	boost::mpl::print<
		entries_type
	> fx;

//

	template <class This>
	static this_type& get(This* _this) {
		return *static_cast<this_type*>(_this);
	}

	template <class This>
	static const this_type& get(const This* _this) {
		return *static_cast<const this_type*>(_this);
	}

	//
	// Master Entry
	//



	struct master_entry : boost::mpl::inherit_linearly<
		entries_type,
		boost::mpl::inherit<
			boost::mpl::_1,
			boost::mpl::_2
		>
	>::type {
		master_entry() {}

		template <class EntityType>
		master_entry(EntityType& entity) {
			boost::mpl::for_each<entries_type>(
				sliced_constructor<master_entry, EntityType>(*this, entity)
			);
		}

		std::string to_string() const {
			std::ostringstream oss;
			boost::mpl::for_each<entries_type>(
				sliced_to_string_or_empty_ostreamer<master_entry>(
					*this, oss, " | ", boost::mpl::size<entries_type>::value
				)
			);

			return oss.str();
		}
	};

	//struct master_entry :
	//	  t0_type::entry
	//	, t1_type::entry
	//	, t2_type::entry {

	//	master_entry() {}

	//	template <class EntityType>
	//	master_entry(EntityType entity) :
	//		  t0_type::entry(entity)
	//		, t1_type::entry(entity)
	//		, t2_type::entry(entity)
	//	{}

	//	std::string to_string() const {
	//		return
	//			to_string_or_empty(static_cast<const t0_type::entry&>(*this)) + " | " +
	//			to_string_or_empty(static_cast<const t1_type::entry&>(*this)) + " | " +
	//			to_string_or_empty(static_cast<const t2_type::entry&>(*this))
	//		;
	//	}
	//};

	//
	// Container
	//




	// TODO: (Maybe) Embed this into the mpl::fold.
	template <typename T>
	struct get_index : boost::mpl::apply<
		typename T::get_index,
		master_entry
	> {};

	typedef typename boost::mpl::fold<
		traits_type,
		boost::mpl::vector<>,
		boost::mpl::if_<
			has_nested_type_get_index<boost::mpl::_2>,
			boost::mpl::push_back<
				boost::mpl::_1,
				get_index<boost::mpl::_2>
			>,
			boost::mpl::_1
		>
	>::type index_vector;

	boost::mpl::print<
		index_vector
	> sd;

	BOOST_MPL_ASSERT((
		boost::is_same<
			typename boost::mpl::at_c<index_vector, 0>::type,
			typename t0_type::get_index::template apply<master_entry>::type
		>
	));



	struct sequenced_index_tag {};

	typedef boost::multi_index::multi_index_container<
		master_entry,
		typename boost::mpl::push_back<
			index_vector,
			boost::multi_index::sequenced<
				boost::multi_index::tag<sequenced_index_tag>
			>
		>::type
	> container_type;


	template <class This>
	static container_type& container(This* _this) {
		return get(_this)._container;
	}

	template <class This>
	static const container_type& container(const This* _this) {
		return get(_this)._container;
	}


	void insert(const master_entry& entry) {
		_container.insert(entry);
		std::cerr << "Inserted: " << entry.to_string() << std::endl;
	}

private:
	container_type _container;
};

//--------------------------------------------------------------------------------------------------

template <class EntityType>
struct pk_tracker : boost::mpl::lambda<
	pk_tracker_impl<boost::mpl::_1, EntityType>
>::type {};

//--------------------------------------------------------------------------------------------------

template <class EntityType>
struct rtuple_data : boost::mpl::lambda<
	rtuple_data_impl<boost::mpl::_1, EntityType>
>::type {};

//--------------------------------------------------------------------------------------------------

template <class EntityType>
struct bitset_load_tracker : boost::mpl::lambda<
	bitset_load_tracker_impl<boost::mpl::_1, EntityType>
>::type {};

//--------------------------------------------------------------------------------------------------

#include <boost/mpl/equal.hpp>

DEFINE_HAS_NESTED_TYPE(ds)

struct fuck_you {
	struct get_index {

	};
};

BOOST_AUTO_TEST_CASE(sandbox) {

	typedef master<
		pk_tracker<Person>,
		bitset_load_tracker<Person>,
		rtuple_data<Person>
	> master_t;

	master_t master;

	auto r0 = master.find(123);
	auto r1 = master.find(1);

	Person john;
	john.ID = 5;
	john.Name = "John Smith";
	john.Height = 1.80;
	john.Fingers = 10;

	{
		master_t::master_entry e;
		e.pk = 1;
		e.data = moneta::traits::to_rtuple(john);
		
		master.insert(e);
	}

	auto r2 = master.find(1);

	master.replace(r2->pk, *r2);
	
	
	auto data0 = master.find(0);
	auto data1 = master.find(1);


	auto r3 = master.find(1);
	if (r3) {
		r3->data;
	}

	int zzz = 0;
}
