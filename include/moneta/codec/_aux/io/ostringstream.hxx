#pragma once
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include "write_string.hxx"

namespace moneta { namespace codec { namespace io {

	template <class OutputIterator>
	struct ostringstream {
		typedef ostringstream this_type;

		bool _good;
		size_t _total_written;

		OutputIterator& _begin;
		OutputIterator  _end;

		ostringstream(OutputIterator& begin, OutputIterator end)
		 : _good(true), _total_written(0), _begin(begin), _end(end) {}

		this_type& operator<<(boost::function<int (OutputIterator, OutputIterator)> callable) {
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
				int result = io::write(_begin, _end, string, false);
				if (result > 0) {
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
				int result = io::write(_begin, _end, string.c_str(), false);
				if (result > 0) {
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

	template <class OutputOutputIterator>
	ostringstream<OutputOutputIterator> make_ostringstream(OutputOutputIterator& begin, OutputOutputIterator end) {
		return ostringstream<OutputOutputIterator>(begin, end);
	}

}}}
