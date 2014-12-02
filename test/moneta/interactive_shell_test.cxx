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
//		std::fill(std::begin(buffer), std::end(buffer), 0);
//		moneta::codec::encode<moneta::codec::debug_dump>(person, std::begin(buffer), std::end(buffer));
//		std::cerr << "visitor::op()<Person>:\n" << buffer << std::endl;
//	}
//
//	void operator()(Address& address) const {
//		char buffer[1024];
//		std::fill(std::begin(buffer), std::end(buffer), 0);
//		moneta::codec::encode<moneta::codec::debug_dump>(address, std::begin(buffer), std::end(buffer));
//		std::cerr << "visitor::op()<Address>:\n" << buffer << std::endl;
//	}
//
//	template <class Entity>
//	void operator()(Entity& entity) const {
//		char buffer[1024];
//		std::fill(std::begin(buffer), std::end(buffer), 0);
//		moneta::codec::encode<moneta::codec::debug_dump>(entity, std::begin(buffer), std::end(buffer));
//		std::cerr << "visitor::op()<default>:\n" << buffer << std::endl;
//	}
//};
//
////BOOST_AUTO_TEST_CASE(interactive_shell_test) {
////	console::shell<> shell;
////
////	shell.add("hello", []() {
////		std::cerr << "Hello, fine sir!" << std::endl;
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
////		moneta::codec::decode_unknown<moneta::codec::shell, entities>(operation(), std::begin(input), std::end(input));
////	});
////
////	console::loop_until_quit_or_exit(shell, std::cin, std::cout);
////}
