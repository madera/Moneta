cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

enable_testing()

project(Moneta CXX)

if(MSVC)
	set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

elseif(APPLE)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	
	include_directories(/usr/local/include)
endif()


include_directories(include)

add_subdirectory(include)
add_subdirectory(tests)
