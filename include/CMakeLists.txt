#
# TODO: Put these macros elsewhere (maybe the final FindMoneta.cmake).
#	Leave this file for manual header listings.
#

set(moneta_include_base "${CMAKE_CURRENT_SOURCE_DIR}/" CACHE STRING INTERNAL)

#
# Gets all existing headers in the directories.
#
macro(moneta_header_files result)
	file(GLOB_RECURSE files "${moneta_include_base}*.hxx")
	set(${result} ${files})
endmacro()

macro(list_directories result base ext)
	set(path "${base}/${ext}")
	#message(STATUS "list_directories: base=[${base}] ext=[${ext}] path=[${path}]")
	
	file(GLOB_RECURSE files "${path}")
	#message(STATUS "list_directories: globed=${files}")
	
	set(dirs "")
	foreach(file ${files})
		get_filename_component(dir ${file} DIRECTORY)
		string(REPLACE "${base}" "" subdir "${dir}")
		list(APPEND dirs ${subdir})
	endforeach()
	
	list(REMOVE_DUPLICATES dirs)
	list(SORT dirs)
	set(${result} ${dirs})
endmacro()

macro(add_source_groups category base ext)
	message(STATUS "Adding source groups: category=[${category}] base=[${base}] ext=[${ext}]")

	list_directories(files ${base} ${ext})	
	#message(STATUS "add_source_groups: directories=${files}")		

	foreach(subdir ${files})
		#message(STATUS "add_source_groups: base=${base} subdir=${subdir}")			

		file(GLOB subfiles "${base}/${subdir}/${ext}")
		#message(STATUS "subfiles: ${subfiles}")

		file(TO_NATIVE_PATH "${category}/${subdir}" branch)
		source_group("${branch}" FILES ${subfiles})
		#message(STATUS "[${branch}] += ${subfiles}")
	endforeach()
endmacro()
