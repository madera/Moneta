#pragma once
#include "soci_includes.hxx"
#include "../../../make_entity.hxx"
#include "../../../traits/is_entity.hxx"
#include "../../../traits/rtuple.hxx"
#include <boost/fusion/view/zip_view.hpp>
#include <boost/fusion/tuple.hpp>

namespace moneta { namespace serialization { namespace soci { namespace detail {

	class fufu {
		::soci::session& _session;
	public:
		fufu(::soci::session& session)
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
			BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size<T>::type, boost::mpl::int_<2> >));
/* XXX */		const int id = boost::fusion::get<1>(pair); // XXX XXX XXX XXX XXX
			boost::fusion::get<0>(pair) = soci_read<
				typename traits::pure_type<typename boost::mpl::at_c<T, 0>::type>::type
			>(_session, id);
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
			BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size<T>::type, boost::mpl::int_<2> >));
			boost::fusion::get<0>(pair) = boost::fusion::get<1>(pair);
		}
	};

	template <class Entity>
	Entity soci_xlate_read(
		::soci::session& session,
		typename moneta::traits::rtuple<Entity>::type& rtuple
	) {
		typedef boost::fusion::vector<
			traits::tuple<Entity>::type&,
			traits::rtuple<Entity>::type&
		> zip_vector_type;

		typename traits::tuple<Entity>::type tuple = make_tuple<Entity>();
		boost::fusion::zip_view<zip_vector_type> zip(zip_vector_type(tuple, rtuple));
		boost::fusion::for_each(zip, fufu(session));
	
		Entity result;
		moneta::traits::to_tie(result) = tuple;
		return result;
	}

}}}}
