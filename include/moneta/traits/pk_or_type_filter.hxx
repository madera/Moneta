#pragma once
#include "pk.hxx"
#include "is_entity.hxx"

namespace moneta { namespace traits {

	namespace detail {

		template <class EntityType, class Enable = void>
		struct pk_or_type;

		template <class NonEntityType>
		struct pk_or_type<
			NonEntityType,
			typename boost::enable_if<
				typename boost::mpl::not_<moneta::traits::is_entity<NonEntityType> >::type
			>::type
		> {
			typedef NonEntityType type;

			type operator()(NonEntityType& value) {
				return value;
			}
		};

		template <class EntityType>
		struct pk_or_type<
			EntityType,
			typename boost::enable_if<moneta::traits::is_entity<EntityType> >::type
		> {
			typedef typename moneta::traits::pk<EntityType>::type type;
		
			type operator()(EntityType& entity) {
				return moneta::traits::extract_pk(entity);
			}
		};

	}

	template <typename T>
	struct pk_or_type_filter {
		typename detail::pk_or_type<T>::type operator()(T& x) {
			return detail::pk_or_type<T>()(x);
		}
	};

}}
