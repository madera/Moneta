#pragma once
#include "pure_type.hxx"
#include "../is_entity.hxx"

namespace moneta { namespace traits { namespace detail {

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

		template<typename T>
		struct result<blanker(T)> : traits::pure_type<T> {};

		template <typename T>
		typename result<blanker(T)>::type
		operator()(T& x) const {
			return make_blank<
				typename result<blanker(T)>::type
			>();
		}
	};

}}}
