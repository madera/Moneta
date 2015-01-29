include_directories(.)

#
# Describe groups for IDEs.
#
if (MSVC)
	add_source_groups("External Moneta" "${moneta_include_base}" *.hxx)
	add_source_groups("Header Files" "${CMAKE_CURRENT_SOURCE_DIR}" *.hxx)
	add_source_groups("Source Files" "${CMAKE_CURRENT_SOURCE_DIR}" *.cxx)
endif()

#
# Collect headers for inclusion.
#
moneta_header_files(moneta_headers)
file(GLOB_RECURSE local_project_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")

find_package(Soci)

#
# Do it.
#
add_executable(core_tests
	${moneta_headers}
	${local_project_headers}
	
	main.cxx
	
	moneta/make_entity_test.cxx
	#moneta/test_sandbox.cxx
	#moneta/interactive_shell_test.cxx
	
	moneta/_aux/mplx_at_c_test.cxx
	moneta/_aux/mplx_flatten_test.cxx
	
	moneta/algorithm/detail/path_test.cxx
	moneta/algorithm/dispatch_entity_test.cxx
	moneta/algorithm/for_each_member_test.cxx
	moneta/algorithm/traverse_test.cxx


	moneta/codec/_aux/tabs_test.cxx
	moneta/codec/_aux/path_tabs_test.cxx
	moneta/codec/_aux/codec_io_test.cxx

	moneta/codec/_aux/io/copy_test.cxx
	moneta/codec/_aux/io/copy_n_test.cxx
	moneta/codec/_aux/io/read_cstr_test.cxx
	moneta/codec/_aux/io/write_cstr_test.cxx


	moneta/codec/typecode_test.cxx
	moneta/codec/encoder_test.cxx
	moneta/codec/decoder_test.cxx
	#
	moneta/codec/xml/detail/xml_traits_test.cxx
	moneta/codec/xml/stateless_xml_encoder_test.cxx
	moneta/codec/xml/stateless_xml_decoder_test.cxx
	moneta/codec/xml/stateless_xml_test.cxx
	#
	moneta/codec/rawbin/rawbin_encoder_test.cxx
	moneta/codec/shell/shell_encoder_test.cxx

#	moneta/codec/encoder_test.cxx
#	moneta/codec/decoder_test.cxx
#	moneta/codec/multi_decoder_test.cxx
#	moneta/codec/rawbin/rawbin_decoder_test.cxx
#	moneta/codec/shell/shell_decoder_test.cxx

	moneta/container/change_trackers/bitset_change_tracker_test.cxx
	moneta/container/change_trackers/hash_change_tracker_test.cxx
	moneta/container/load_trackers/bitset_load_tracker_test.cxx
	moneta/container/meta_set_test.cxx
	moneta/container/rcontext_test.cxx
	moneta/container/rtuple_set_test.cxx

	moneta/lexical/dispatch_member_test.cxx
	moneta/lexical/lexical_dispatch_entity_test.cxx
	moneta/lexical/set_value_test.cxx

	#		
	# TODO: Make these selectable only if FindSOCI finds it.
	#
	#moneta/serialization/soci/detail/soci_fixture.cxx
	#moneta/serialization/soci/soci_count_test.cxx
	#moneta/serialization/soci/soci_create_test.cxx
	#moneta/serialization/soci/soci_delete_test.cxx
	#moneta/serialization/soci/soci_list_test.cxx
	#moneta/serialization/soci/soci_read_test.cxx

	moneta/traits/detail/deref_if_unary_test.cxx
	moneta/traits/detail/has_member_trait_test.cxx
	moneta/traits/detail/indices_of_test.cxx
	moneta/traits/detail/is_functor_callable_test.cxx
	moneta/traits/detail/sub_tie_vector_test.cxx
	moneta/traits/detail/tie_vector_test.cxx	
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

	#moneta/pp/detail/cat_include_name_test.cxx
	moneta/pp/detail/includer_test.cxx
	moneta/pp/pp_test.cxx

	moneta/sql/generators/delete_test.cxx
	moneta/sql/generators/insert_test.cxx
	moneta/sql/generators/select_test.cxx
	moneta/sql/traits/field_names_test.cxx
	moneta/sql/traits/table_name_test.cxx
)

target_link_libraries(core_tests ${Boost_LIBRARIES})

add_test(core_tests core_tests)
