#include "stdafx.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <console/shell.hxx>

template <class Shell, class Input, class Output>
void loop_until_quit_or_exit(Shell& shell, Input& input, Output& output) {
	bool done = false;
	while (!done) {
		output << shell.prompt();

		std::string line;
		std::getline(input, line);

		try {
			shell(line);
		}
		catch (std::exception& ex) {
			std::cerr << "TOP EXCEPTION: " << ex.what() << std::endl;
		}

		if (line == "exit" || line == "quit") {
			done = true;
		}
	}
}

BOOST_AUTO_TEST_CASE(interactive_shell_test) {
	console::shell<> shell;

	shell.add("hello", []() {
		std::cerr << "Hello, motherfucker!" << std::endl;
	});

	loop_until_quit_or_exit(shell, std::cin, std::cout);
}
