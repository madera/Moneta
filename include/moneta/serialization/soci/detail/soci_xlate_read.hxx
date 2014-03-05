#pragma once

namespace moneta { namespace serialization { namespace soci { namespace detail {

	class fufu {
		::soci::session& _session;
	public:
		fufu(::soci::session& session)
		: _session(session) {
		}

		template <typename T>
		typename boost::enable_if<fucker::is_entity<typename mpl::at_c<T, 0>::type>, void>::type
		operator()(T& pair) const {
			BOOST_MPL_ASSERT((mpl::equal_to<mpl::size<T>::type, mpl::int_<2> >));
/* XXX */		const int id = boost::fusion::get<1>(pair); // XXX XXX XXX XXX XXX
			boost::fusion::get<0>(pair) = soci_read<
				typename traits::pure_type<typename mpl::at_c<T, 0>::type>::type
			>(_session, id);
		}

		template <typename T>
		typename boost::enable_if<mpl::not_<fucker::is_entity<typename mpl::at_c<T, 0>::type> >, void>::type
		operator()(T& pair) const {
			BOOST_MPL_ASSERT((mpl::equal_to<mpl::size<T>::type, mpl::int_<2> >));
			boost::fusion::get<0>(pair) = boost::fusion::get<1>(pair);
		}
	};

	template <class EntityType>
	EntityType soci_xlate_read(::soci::session& session, typename traits::sql::db_tuple<EntityType>::type& db_tuple) {
		typedef boost::fusion::vector<
			traits::tuple<EntityType>::type&,
			traits::sql::db_tuple<EntityType>::type&
		> zip_vector_type;

		traits::tuple<EntityType>::type tuple = make_tuple<EntityType>();
		boost::fusion::zip_view<zip_vector_type> zip(zip_vector_type(tuple, db_tuple));
		boost::fusion::for_each(zip, fufu(session));
	
		EntityType result;
		to_tie(result) = tuple;
		return result;
	}

}}}}
