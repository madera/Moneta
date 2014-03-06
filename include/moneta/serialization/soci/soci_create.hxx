#pragma once
#include "detail/soci_includes.hxx"
#include "../../make_entity.hxx"
#include "../../traits/is_entity.hxx"
#include "../../traits/extract_pk.hxx"
#include "../../sql/traits/db_tuple.hxx"
#include "../../sql/generators/insert.hxx"
#include <boost/fusion/view/zip_view.hpp>
#include <boost/fusion/tuple.hpp>

namespace moneta { namespace serialization { namespace soci {

	// TODO: Move this somewhere else.
	template <class EntityType>
	class incremental_integer_generator {
		::soci::session& _session;
	public:
		incremental_integer_generator(::soci::session& session)
		 : _session(session) {
		}

		typename traits::pk<EntityType>::type operator()() const {
			// Only single-field primary keys are supported.
			//BOOST_MPL_ASSERT((boost::mpl::and_<
			//	boost::mpl::not_<
			//		traits::is_fusion_vector<typename traits::pk<EntityType>::type>
			//	>,
			//	boost::mpl::equal_to<
			//		boost::mpl::size<typename traits::pk<EntityType>::type>,
			//		boost::mpl::int_<1>::type
			//	>
			//>));

			const std::string field = fucker::traits::sql::get_pk_field_names<EntityType>()[0];
			const std::string table = fucker::traits::sql::table_name<fucker::traits::pure_type<EntityType>::type>::get(); // FIXME: Make this nice.

			//int id = 0;
			boost::optional<int> id;
			try {
				_session << boost::format("select max(%s) from %s") % field % table,
					    ::soci::into(id);
			} catch (...) {
				// FIXME: Make this better.
				id = 0;
			}

			return typename fucker::traits::pk<EntityType>::type(
				id? ++id.get() : 1
			);
		}
	};

	class huhu {
		::soci::session& _session;
	public:
		huhu(::soci::session& session)
		: _session(session) {
		}

		template <typename T>
		typename boost::enable_if<
			traits::is_entity<
			typename boost::mpl::at_c<T, 0>::type
			>,
			void
		>::type
		operator()(T& pair) const {
			BOOST_MPL_ASSERT((mpl::equal_to<mpl::size<T>::type, mpl::int_<2> >));
			boost::fusion::get<1>(pair) = soci_create<
				typename traits::pure_type<typename mpl::at_c<T, 0>::type>::type
			>(_session, boost::fusion::get<0>(pair));
		}

		template <typename T>
		typename boost::enable_if<
			boost::mpl::not_<
				traits::is_entity<
					typename boost::mpl::at_c<T, 0>::type
				>
			>,
			void
		>::type
		operator()(T& pair) const {
			BOOST_MPL_ASSERT((
				boost::mpl::equal_to<
					boost::mpl::size<T>::type,
					boost::mpl::int_<2>
				>
			));

			boost::fusion::get<1>(pair) = boost::fusion::get<0>(pair);
		}
	};

	template <class EntityType>//, class SociIdGenerator = boost::function<int()> >
	typename traits::pk<EntityType>::type
	soci_create(::soci::session& session, EntityType& entity) {
		//, SociIdGenerator id_generator = incremental_integer_generator<EntityType>(session)) {
		//// TODO: Allow specializations.
		//SociIdGenerator id_generator = incremental_integer_generator<EntityType>(session);

		//if (traits::has_empty_pk(entity)) {
		//	traits::extract_pk(entity) = id_generator();
		//}

		traits::tie<EntityType>::type entity_tuple = moneta::traits::to_tie<EntityType>(entity);
		sql::traits::db_tuple<EntityType>::type db_tuple;

		typedef boost::fusion::vector<
			traits::tie<EntityType>::type&,
			sql::traits::db_tuple<EntityType>::type&
		> zip_vector_type;

		boost::fusion::zip_view<zip_vector_type> zip(zip_vector_type(entity_tuple, db_tuple));
		boost::fusion::for_each(zip, huhu(session));

		session << sql::generators::insert_into_table<EntityType>(), ::soci::use(db_tuple);

		return traits::extract_pk(entity);
	}

}}}
