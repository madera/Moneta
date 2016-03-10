// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "stdafx.h"
//#include <console/shell.hxx>
//
//// TODO: Clean this up!
//// For tests:
//#include <moneta/codec/shell/shell_decoder.hxx>
//#include <moneta/codec/xml/xml_encoder.hxx>
//#include <moneta/codec/debug_dump/debug_dump_encoder.hxx>
//#include "model/Person.hxx"
//#include "model/Composite.hxx"
//#include "model/Cat.hxx"
//
//struct operation {
//	void operator()(Person& person) const {
//		char buffer[1024];
//		std::fill(boost::begin(buffer), boost::end(buffer), 0);
//		moneta::codec::encode<moneta::codec::debug_dump>(person, boost::begin(buffer), boost::end(buffer));
//		std::cerr << "visitor::op()<Person>:\n" << buffer << boost::endl;
//	}
//
//	void operator()(Address& address) const {
//		char buffer[1024];
//		std::fill(boost::begin(buffer), boost::end(buffer), 0);
//		moneta::codec::encode<moneta::codec::debug_dump>(address, boost::begin(buffer), boost::end(buffer));
//		std::cerr << "visitor::op()<Address>:\n" << buffer << boost::endl;
//	}
//
//	template <class Entity>
//	void operator()(Entity& entity) const {
//		char buffer[1024];
//		std::fill(boost::begin(buffer), boost::end(buffer), 0);
//		moneta::codec::encode<moneta::codec::debug_dump>(entity, boost::begin(buffer), boost::end(buffer));
//		std::cerr << "visitor::op()<default>:\n" << buffer << boost::endl;
//	}
//};
//
////BOOST_AUTO_TEST_CASE(interactive_shell_test) {
////	console::shell<> shell;
////
////	shell.add("hello", []() {
////		std::cerr << "Hello, fine sir!" << boost::endl;
////	});
////
////	shell.add("decode", [](const std::vector<std::string>& args) {
////		std::string input;
////		for (size_t i=1; i<args.size(); ++i) {
////			input += args[i];
////			input += ' ';
////		}
////
////		typedef boost::mpl::vector5<Person, Address, Cat, Composite, Dog> entities;
////		moneta::codec::decode_unknown<moneta::codec::shell, entities>(operation(), boost::begin(input), boost::end(input));
////	});
////
////	console::loop_until_quit_or_exit(shell, std::cin, std::cout);
////}
