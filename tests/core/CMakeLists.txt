include_directories(.)

###
### TODO: ORGANIZE THESE ALPHABETICALLY
###

add_executable(core_tests
	main.cxx
	
	moneta/_aux/mplx_at_c_test.cxx
	moneta/_aux/mplx_flatten_test.cxx
	
	moneta/algorithm/dispatch_entity_test.cxx
	moneta/algorithm/for_each_member_test.cxx
	moneta/algorithm/traverse_ng_test.cxx
	moneta/algorithm/traverse_test.cxx

	moneta/container/change_trackers/bitset_change_tracker_test.cxx
	moneta/container/change_trackers/hash_change_tracker_test.cxx
	moneta/container/load_trackers/bitset_load_tracker_test.cxx
	moneta/container/meta_set_test.cxx
	moneta/container/rcontext_test.cxx
	moneta/container/rtuple_set_test.cxx


	moneta/lexical/dispatch_member_test.cxx
	moneta/lexical/lexical_dispatch_entity_test.cxx
	moneta/lexical/set_value_test.cxx
	
	
	moneta/traits/detail/deref_if_unary_test.cxx
	moneta/traits/detail/has_member_trait_test.cxx
	moneta/traits/detail/indices_of_test.cxx
	moneta/traits/detail/is_functor_callable_test.cxx
	moneta/traits/detail/sub_tie_vector_test.cxx
	moneta/traits/detail/tie_vector_test.cxx	
	#
	moneta/traits/empty_pk_test.cxx
	moneta/traits/extract_pk_test.cxx
	moneta/traits/fixed_values_test.cxx
	moneta/traits/is_container_test.cxx
	moneta/traits/is_entity_test.cxx
	moneta/traits/member_names_test.cxx
	moneta/traits/members_test.cxx
	moneta/traits/pk_member_names_test.cxx
	moneta/traits/pk_rtie_test.cxx
	moneta/traits/pk_rtuple_test.cxx
	moneta/traits/pk_test.cxx
	moneta/traits/pk_tie_test.cxx
	moneta/traits/rtie_test.cxx
	moneta/traits/rtuple_test.cxx
	moneta/traits/tie_test.cxx
	moneta/traits/to_entity_test.cxx
	moneta/traits/to_pk_rtuple_test.cxx
	moneta/traits/to_rtie_test.cxx
	moneta/traits/to_rtuple_test.cxx
	moneta/traits/to_tie_test.cxx
	moneta/traits/tuple_test.cxx


	moneta/sql/generators/delete_test.cxx
	moneta/sql/generators/insert_test.cxx
	moneta/sql/generators/select_test.cxx
	#
	moneta/sql/traits/field_names_test.cxx
	moneta/sql/traits/table_name_test.cxx


)

target_link_libraries(core_tests ${Boost_LIBRARIES})
