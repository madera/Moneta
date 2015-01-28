#pragma once
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include "io/copy.hxx"

namespace moneta { namespace codec {

	namespace detail {

		// TODO: Make this work with all forward iterators.
		template <class InputIterator, class OutputIterator>
		int copy(InputIterator first, InputIterator last, OutputIterator begin, OutputIterator end) {
			int written = 0;
		
			for ( ; first != last && begin != end; ++written) {
				*begin++ = *first++;
			}

			// Check if there was data left.
			if (first != last && begin == end) {
				return 0 - std::distance(first, last);
			}

			return written;
		}

		template <class Iterator>
		int copy(const std::string& string, Iterator begin, Iterator end) {
			return copy(std::begin(string), std::end(string), begin, end);
		}

		// TODO: Validate this non-reference Iterator state.
		template <class Iterator>
		struct ostringstream {
			typedef ostringstream this_type;

			bool _good;
			size_t _total_written;

			Iterator _begin;
			Iterator _end;

			ostringstream(Iterator begin, Iterator end)
			 : _good(true), _total_written(0), _begin(begin), _end(end) {}

			this_type& operator<<(boost::function<int (Iterator, Iterator)> callable) {
				if (_good) {
					int result = callable(_begin, _end);
					if (result > 0) {
						_begin += result;
						_total_written += result;
					} else if (result == 0) {
					} else if (result < 0) {
						_good = false;
						_total_written = result;
					}
				}

				return *this;
			}

			this_type& operator<<(const char* string) {
				if (_good) {
					// XXX: strlen() is slow!!
					// TODO: Implement fast c-string copy.
					int result = copy(string, string + strlen(string), _begin, _end);
					if (result > 0) {
						_begin += result;
						_total_written += result;
					} else if (result == 0) {
					} else if (result < 0) {
						_good = false;
						_total_written = result;
					}
				}

				return *this;
			}

			this_type& operator<<(const char c) {
				if (_good) {
					if (_begin != _end) {
						*_begin++ = c;
						++_total_written;
					} else {
						_good = false;
						_total_written = -1;
					}
				}

				return *this;
			}

			this_type& operator<<(const std::string& string) {
				if (_good) {
					int result = copy(string, _begin, _end);
					if (result > 0) {
						_begin += result;
						_total_written += result;
					} else if (result == 0) {
					} else if (result < 0) {
						_good = false;
						_total_written = result;
					}
				}

				return *this;
			}

			this_type& operator<<(const std::vector<std::string>& string_vector) {
				std::vector<std::string>::const_iterator itr = string_vector.begin();
				for (; itr != string_vector.end(); ++itr) {
					if (_good) {
						*this << *itr;

						if (itr + 1 != string_vector.end()) {
							*this << ',';
						}
					}
				}

				return *this;
			}

			template <class T>
			typename boost::enable_if<boost::is_pod<T>, this_type&>::type
			operator<<(const T& value) {
				*this << boost::lexical_cast<std::string>(value);
				return *this;
			}

			operator int() const {
				return _total_written;
			}
		};

		template <class Iterator>
		ostringstream<Iterator> make_ostringstream(Iterator begin, Iterator end) {
			return ostringstream<Iterator>(begin, end);
		}
	}

}}
