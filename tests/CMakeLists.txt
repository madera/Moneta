# [===========================================================================]
# [                                M o n e t a                                ]
# [---------------------------------------------------------------------------]
# [                                                                           ]
# [                          Copyright (C) 2005-2015                          ]
# [                      Rodrigo Madera <madera@acm.org>                      ]
# [                                                                           ]
# [---------------------------------------------------------------------------]
# [         Distributed under the Boost Software License, Version 1.0         ]
# [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
# [===========================================================================]

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.55.0 COMPONENTS system thread test_exec_monitor REQUIRED)
if (Boost_FOUND)
	message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost libs    : ${Boost_LIBRARIES}")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03 -stdlib=libc++ -Wall -Wextra -Os")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03 -Wall -Wextra -Os")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /x /y /z")
endif()

add_subdirectory(core)
