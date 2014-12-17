include_directories(.)

add_executable(core_tests
	main.cxx
	
	moneta/_aux/mplx_at_c_test.cxx
	moneta/_aux/mplx_flatten_test.cxx
	
	moneta/algorithm/dispatch_entity_test.cxx
	moneta/algorithm/for_each_member_test.cxx
	moneta/algorithm/traverse_ng_test.cxx
	moneta/algorithm/traverse_test.cxx
)

target_link_libraries(core_tests ${Boost_LIBRARIES})
