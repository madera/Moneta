#pragma once

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		// Generic read
		//
		template <class T>
		struct read {
			template <class IteratorType>
			IteratorType operator()(IteratorType begin, IteratorType end, T& value) {
				BOOST_ASSERT(std::distance(begin, end) >= sizeof(int));
				std::copy(begin, begin + sizeof(T), reinterpret_cast<char*>(&value));
				return begin + sizeof(T);
			}
		};

		// Generic write
		//
		template <class T>
		struct write {
			template <class IteratorType>
			IteratorType operator()(const T& value, IteratorType begin, IteratorType end) {
				BOOST_ASSERT(std::distance(begin, end) >= sizeof(T)); // XXX
				const char* data = reinterpret_cast<const char*>(&value);
				std::copy(data, data + sizeof(T), begin);
				return begin + sizeof(T);
			}
		};

		struct write_string {
			template <class IteratorType>
			IteratorType operator()(const std::string& value, IteratorType begin, IteratorType end) {
				BOOST_ASSERT(std::distance(begin, end) >= value.size()); // XXX
				const char* data = value.c_str();
				std::copy(data, data + value.size(), begin);
				*(begin + value.size()) = 0;
				return begin + value.size() + 1;
			}
		};

		template <>
		struct write<std::string> : write_string {};

		template <>
		struct write<const std::string> : write_string {};

	}

	//
	//
	//

	template <class T, class IteratorType>
	IteratorType read_raw(IteratorType begin, IteratorType end, T& value) {
		return detail::read<T>()(begin, end, value);
	}

	template <class T, class IteratorType>
	IteratorType write_raw(T& value, IteratorType begin, IteratorType end) {
		std::cout << "-> " << typeid(T).name() << std::endl;
		return detail::write<T>()(value, begin, end);
	}

}}}
