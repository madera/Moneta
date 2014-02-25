#pragma once
#include "traits/is_entity.hxx"
#include "traits/tuple.hxx"
#include "traits/to_tie.hxx"
#include <boost/fusion/algorithm/transformation/transform.hpp>

namespace moneta {

	namespace detail {
		template <typename T>
		typename boost::enable_if<traits::is_entity<T>, T>::type
		make_blank() {
			return make_entity<T>();
		}

		template <typename T>
		typename boost::disable_if<traits::is_entity<T>, T>::type
		make_blank() {
			return T();
		}

		struct blanker {
			template<typename Sig>
			struct result;

			template<typename U>
			struct result<blanker(U)> // XXX: use pure_type
				: boost::remove_const<typename boost::remove_reference<U>::type> {};

			template <typename T>
			typename result<blanker(T)>::type
			operator()(T& x) const {
				return make_blank<
					typename result<blanker(T)>::type
				>();
			}
		};

	}

	template <class EntityType>
	typename traits::tuple<EntityType>::type make_tuple() {
		traits::tuple<EntityType>::type result;
		return boost::fusion::transform(result, detail::blanker());
	}

	template <class EntityType>
	EntityType make_entity() {
		EntityType result;
		traits::to_tie<EntityType>(result) = make_tuple<EntityType>();
		return result;
	}
}
