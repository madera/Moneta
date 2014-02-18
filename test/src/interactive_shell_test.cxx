#include "stdafx.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <console/shell.hxx>

BOOST_AUTO_TEST_CASE(interactive_shell_test) {
	console::shell<> shell;

	shell.add("hello", []() {
		std::cerr << "Hello, motherfucker!" << std::endl;
	});

	console::loop_until_quit_or_exit(shell, std::cin, std::cout);
}
