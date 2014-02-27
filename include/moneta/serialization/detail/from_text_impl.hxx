// TODO: Boost.PP this file.
#pragma once
#include "../../traits/members.hxx"
#include "../../traits/member_pointers.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/mpl/size.hpp>

namespace moneta { namespace serialization { namespace detail {

	template <class EntityType, int Size = boost::mpl::size<traits::members<EntityType>::type>::value>
	struct from_text_impl;

	template <class EntityType>
	struct from_text_impl<EntityType, 0> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
		}
	};

	template <class EntityType>
	struct from_text_impl<EntityType, 1> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
			using namespace boost;

			typedef traits::members<EntityType>::type members;
			auto memptrs = traits::member_pointers<EntityType>::get();

			if (index == 0) {
				result.*fusion::at_c<0>(memptrs) = lexical_cast<mpl::at_c<members, 0>::type::result_type>(value);
			}
		}
	};

	template <class EntityType>
	struct from_text_impl<EntityType, 2> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
			using namespace boost;

			typedef traits::members<EntityType>::type members;
			auto memptrs = traits::member_pointers<EntityType>::get();

			if (index == 0) {
				result.*fusion::at_c<0>(memptrs) = lexical_cast<mpl::at_c<members, 0>::type::result_type>(value);
			} else if (index == 1) {
				result.*fusion::at_c<1>(memptrs) = lexical_cast<mpl::at_c<members, 1>::type::result_type>(value);
			}
		}
	};

	template <class EntityType>
	struct from_text_impl<EntityType, 3> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
			using namespace boost;

			typedef traits::members<EntityType>::type members;
			auto memptrs = traits::member_pointers<EntityType>::get();

			if (index == 0) {
				result.*fusion::at_c<0>(memptrs) = lexical_cast<mpl::at_c<members, 0>::type::result_type>(value);
			} else if (index == 1) {
				result.*fusion::at_c<1>(memptrs) = lexical_cast<mpl::at_c<members, 1>::type::result_type>(value);
			} else if (index == 2) {
				result.*fusion::at_c<2>(memptrs) = lexical_cast<mpl::at_c<members, 2>::type::result_type>(value);
			}
		}
	};

	template <class EntityType>
	struct from_text_impl<EntityType, 4> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
			using namespace boost;

			typedef traits::members<EntityType>::type members;
			auto memptrs = traits::member_pointers<EntityType>::get();

			if (index == 0) {
				result.*fusion::at_c<0>(memptrs) = lexical_cast<mpl::at_c<members, 0>::type::result_type>(value);
			} else if (index == 1) {
				result.*fusion::at_c<1>(memptrs) = lexical_cast<mpl::at_c<members, 1>::type::result_type>(value);
			} else if (index == 2) {
				result.*fusion::at_c<2>(memptrs) = lexical_cast<mpl::at_c<members, 2>::type::result_type>(value);
			} else if (index == 3) {
				result.*fusion::at_c<3>(memptrs) = lexical_cast<mpl::at_c<members, 3>::type::result_type>(value);
			}
		}
	};

	template <class EntityType>
	struct from_text_impl<EntityType, 5> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
			using namespace boost;

			typedef traits::members<EntityType>::type members;
			auto memptrs = traits::member_pointers<EntityType>::get();

			if (index == 0) {
				result.*fusion::at_c<0>(memptrs) = lexical_cast<mpl::at_c<members, 0>::type::result_type>(value);
			} else if (index == 1) {
				result.*fusion::at_c<1>(memptrs) = lexical_cast<mpl::at_c<members, 1>::type::result_type>(value);
			} else if (index == 2) {
				result.*fusion::at_c<2>(memptrs) = lexical_cast<mpl::at_c<members, 2>::type::result_type>(value);
			} else if (index == 3) {
				result.*fusion::at_c<3>(memptrs) = lexical_cast<mpl::at_c<members, 3>::type::result_type>(value);
			} else if (index == 4) {
				result.*fusion::at_c<4>(memptrs) = lexical_cast<mpl::at_c<members, 4>::type::result_type>(value);
			}
		}
	};

	template <class EntityType>
	struct from_text_impl<EntityType, 6> {
		template <class EntityType>
		void operator()(EntityType& result, const size_t index, const std::string value) const {
			using namespace boost;

			typedef traits::members<EntityType>::type members;
			auto memptrs = traits::member_pointers<EntityType>::get();

			if (index == 0) {
				result.*fusion::at_c<0>(memptrs) = lexical_cast<mpl::at_c<members, 0>::type::result_type>(value);
			} else if (index == 1) {
				result.*fusion::at_c<1>(memptrs) = lexical_cast<mpl::at_c<members, 1>::type::result_type>(value);
			} else if (index == 2) {
				result.*fusion::at_c<2>(memptrs) = lexical_cast<mpl::at_c<members, 2>::type::result_type>(value);
			} else if (index == 3) {
				result.*fusion::at_c<3>(memptrs) = lexical_cast<mpl::at_c<members, 3>::type::result_type>(value);
			} else if (index == 4) {
				result.*fusion::at_c<4>(memptrs) = lexical_cast<mpl::at_c<members, 4>::type::result_type>(value);
			} else if (index == 5) {
				result.*fusion::at_c<5>(memptrs) = lexical_cast<mpl::at_c<members, 5>::type::result_type>(value);
			}
		}
	};

}}}
