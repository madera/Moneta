//
// TODO: Review where to put this file.
//

// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2016                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include <boost/circular_buffer.hpp>

namespace moneta { namespace codec {

        template <class Decoder, size_t InitialSize = 1024>
        struct buffered_decoder {
                typedef Decoder decoder_type;
        public:
                buffered_decoder()
                 : _buffer(InitialSize) {
                }

                size_t size() const {
                        return _buffer.size();
                }

                size_t capacity() const {
                        return _buffer.capacity();
                }

                template <typename T>
                size_t append_bytes(const T* data, const size_t size) {
                        while (_buffer.capacity() - _buffer.size() < size) {
                                std::cout << "--- resizing to: "
                                          << 2*_buffer.capacity() << std::endl;
                                _buffer.set_capacity(2*_buffer.capacity());
                        }

                        std::copy(data, data+size, std::back_inserter(_buffer));
                        return _buffer.size();
                }

                size_t decode() {
                        if (_buffer.empty() || _buffer.size() < 2)
                                return 0;

                        typename decoder_type::variant_type variant;
                        int consumed = _decoder(
                                _buffer.linearize(),
                                _buffer.linearize() + _buffer.size(),
                                variant
                        );

                        if (consumed <= 0)
                                return 0;

                        std::string result(&_buffer[0], &_buffer[0] + consumed);
                        for (size_t i=0; i<consumed; ++i)
                                _buffer.pop_front();

                        std::cout << "+++ decoded: [" << result << "]" << std::endl;
                        return consumed;
                }

#if 1
                void debug_dump() const {
                        std::cout << "Buffer={"
                                  << " size=" << size()
                                  << " capacity=" << capacity()
                                  << " data='" << debug_buffer_str() << "'"
                                  << " }" << std::endl;
                }

                std::string debug_buffer_str() const {
                        std::string result;
                        std::cerr << "d=" << std::distance(_buffer.begin(), _buffer.end()) << std::endl;
                        std::copy(_buffer.begin(), _buffer.end(), std::back_inserter(result));
                        return result;
                }
#endif
        private:
                decoder_type _decoder;
                boost::circular_buffer<boost::uint8_t> _buffer;
        };

}}
