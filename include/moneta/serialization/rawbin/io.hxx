#pragma once

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <class T>
		struct read_raw_impl {
			template <class IteratorType>
			IteratorType operator()(IteratorType begin, IteratorType end, T& value) {
				BOOST_ASSERT(std::distance(begin, end) >= sizeof(int));
				std::copy(begin, begin + sizeof(T), reinterpret_cast<char*>(&value));
				return begin + sizeof(T);
			}
		};

		template <class T>
		struct write_raw_impl {
			template <class IteratorType>
			IteratorType operator()(const T& value, IteratorType begin, IteratorType end) {
				BOOST_ASSERT(std::distance(begin, end) >= sizeof(T)); // XXX
				const char* data = reinterpret_cast<const char*>(&value);
				std::copy(data, data + sizeof(T), begin);
				return begin + sizeof(T);
			}
		};

		template <>
		struct write_raw_impl<const std::string> {
			template <class IteratorType>
			IteratorType operator()(const std::string& value, IteratorType begin, IteratorType end) {
				BOOST_ASSERT(std::distance(begin, end) >= value.size()); // XXX
				const char* data = value.c_str();
				std::copy(data, data + value.size(), begin);
				*(begin + value.size()) = 0;
				return begin + value.size() + 1;
			}
		};
	}

	template <class T, class IteratorType>
	IteratorType read_raw(IteratorType begin, IteratorType end, T& value) {
		return detail::read_raw_impl<T>()(begin, end, value);
	}

	template <class T, class IteratorType>
	IteratorType write_raw(T& value, IteratorType begin, IteratorType end) {
		return detail::write_raw_impl<T>()(value, begin, end);
	}

}}}
