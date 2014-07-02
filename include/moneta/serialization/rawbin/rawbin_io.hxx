#pragma once

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <class T, class Enable = void>
		struct read_impl;

		template <class T, class Enable = void>
		struct write_impl;



		template <class T>
		struct read_impl<T, typename boost::enable_if<boost::is_arithmetic<T> >::type> {
			template <class IteratorType>
			int operator()(IteratorType begin, IteratorType end, T& value) {
				BOOST_ASSERT(std::distance(begin, end) >= sizeof(int));
				std::copy(begin, begin + sizeof(T), reinterpret_cast<char*>(&value));
				return sizeof(T);
			}
		};

		template <class T>
		struct write_impl<T, typename boost::enable_if<boost::is_arithmetic<T> >::type> {
			template <class IteratorType>
			IteratorType operator()(const T& value, IteratorType begin, IteratorType end) {
				BOOST_ASSERT(size_t(std::distance(begin, end)) >= sizeof(T));
				const char* data = reinterpret_cast<const char*>(&value);
				std::copy(data, data + sizeof(T), begin);
				return begin + sizeof(T);
			}
		};

		// TODO: read_impl for std::string

		template <class T>
		struct write_impl<
			T,
			typename boost::enable_if<
				boost::is_same<
					typename boost::remove_const<T>::type,
					std::string
				>
			>::type
		> {
			template <class IteratorType>
			IteratorType operator()(const std::string& value, IteratorType begin, IteratorType end) {
				BOOST_ASSERT(size_t(std::distance(begin, end)) >= value.size()); // XXX
				const char* data = value.c_str();
				std::copy(data, data + value.size(), begin);
				*(begin + value.size()) = 0;
				return begin + value.size() + 1;
			}
		};

	}

	//
	//
	//

	template <class T, class IteratorType>
	int read(IteratorType begin, IteratorType end, T& value) {
		return detail::read_impl<T>()(begin, end, value);
	}

	template <class T, class IteratorType>
	IteratorType write(T& value, IteratorType begin, IteratorType end) {
		return detail::write_impl<T>()(value, begin, end);
	}

}}}
