#include "stdafx.h"
#include <moneta/traits/pk.hxx>
#include <moneta/traits/rtuple.hxx>
#include <moneta/traits/extract_pk.hxx>
#include <boost/call_traits.hpp>
#include "model/Person.hxx"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/optional.hpp>

template <class Master>
struct bitset_load_tracker {
	typedef bitset_load_tracker this_type;

	struct entry {
		typedef int state_type;
		state_type load_state;

		entry() {
			load_state = 10;
		}
	};

	template <class MasterEntry>
	struct get_index {
		typedef boost::multi_index::hashed_unique<
			boost::multi_index::tag<this_type>,
			boost::multi_index::member<
				entry,
				int,
				&entry::load_state
			>
		> type;
	};

	int is_loaded(const size_t i) {
		return 321;
	}
};

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
	};

	template <class MasterEntry>
	struct get_index {
		typedef boost::multi_index::hashed_unique<
			boost::multi_index::tag<this_type>,
			boost::multi_index::member<
				entry,
				typename entry::state_type,
				&entry::pk
			>
		> type;
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
	};

	template <class MasterEntry>
	struct get_index {
		typedef boost::multi_index::sequenced< // XXX: WRONG. Support null indexes!
			boost::multi_index::tag<this_type>
		> type;
	};

	template <class LazyHack = Master>
	boost::optional<typename entry::state_type>
	data(typename boost::call_traits<typename moneta::traits::pk<EntityType>::type>::param_type pk) const {
		boost::optional<Master::master_entry> entry = Master::get(this).find(pk);
		return (entry)? boost::optional<typename entry::state_type>(entry->data) :
				boost::optional<typename entry::state_type>();
	}
};





template <
	class T0,
	class T1
>
struct master :
	boost::mpl::apply<T0, master<T0, T1> >::type,
	boost::mpl::apply<T1, master<T0, T1> >::type
{
	typedef master this_type;

	typedef typename boost::mpl::apply<T0, master<T0, T1> >::type t0_type;
	typedef typename boost::mpl::apply<T1, master<T0, T1> >::type t1_type;

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

	struct master_entry :
		  t0_type::entry
		, t1_type::entry {

		master_entry() {}

		template <class EntityType>
		master_entry(EntityType entity) :
			  t0_type::entry(entity)
			, t1_type::entry(entity)
		{}
	};

	//
	// Container
	//

	typedef boost::multi_index::multi_index_container<
		master_entry,
		boost::multi_index::indexed_by<
			  typename t0_type::template get_index<master_entry>::type
			, typename t1_type::template get_index<master_entry>::type
		>
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
	}

private:
	container_type _container;
};
















template <class EntityType>
struct pk_tracker : boost::mpl::lambda<
	pk_tracker_impl<boost::mpl::_1, EntityType>
>::type {};

template <class EntityType>
struct rtuple_data : boost::mpl::lambda<
	rtuple_data_impl<boost::mpl::_1, EntityType>
>::type {};



BOOST_AUTO_TEST_CASE(sandbox) {

	typedef master<
		pk_tracker<Person>,
		rtuple_data<Person>
	> master_t;

	master_t master;

	auto r0 = master.find(123);
	auto r1 = master.find(1);

	{
		master_t::master_entry e;
		e.pk = 1;

		master.insert(e);
	}

	auto r2 = master.find(1);

	master.replace(r2->pk, *r2);
	
	
	auto data0 = master.data(0);
	auto data1 = master.data(1);


	auto r3 = master.find(1);



	int zzz = 0;
}
