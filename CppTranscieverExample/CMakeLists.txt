cmake_minimum_required(VERSION "3.16")

###############################################################################
## config #####################################################################
###############################################################################

set(debug_level 3 CACHE STRING "debug level.")
set(optimization_level 0 CACHE STRING "optimization level.")

# set CMAKE_TOOLCHAIN_FILE to use specified toolchain.
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../toolchains/arm-linux-gnueabi.cmake")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../toolchains/native.cmake" CACHE STRING "Toolchain file.")

# set cpp standard
set(CMAKE_CXX_STANDARD 17)

# for single-config generators (makefile is), this variable should be set.
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")

# set project name and version.
project("ProtoExample" VERSION "0.1.0"
	DESCRIPTION "A sample tcp server project with cmake build system."
)

# export compile commands for source code indexers.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# global options should be set before adding a target (executable or library).
# # optional global compiler options.
# add_compile_options(
#     -std=c++17 -Wall -Wextra -Wconversion -Wfloat-equal
# )

# enable optimizations and debugging.
add_compile_options(
	-O${optimization_level} -g${debug_level}
)

# display project-related information.
message(NOTICE "-- CMAKE_CROSSCOMPILING = ${CMAKE_CROSSCOMPILING}")

###############################################################################
## variables ##################################################################
###############################################################################

# set project target names.
set(receiver "Receiver")
set(transmitter "Transmitter")
set(library "ProtoGeneration")

foreach(executable ${receiver} ${transmitter})
	# dir variable names made unique to project since having
	# the same list name with append option accross different
	# projects causes appending dirs to the same list.

	# set project source directories.
	list(APPEND src_dirs_${executable}
		"${executable}/src"
	)

	# set project include directories.
	list(APPEND inc_dirs_${executable}
		"${executable}/src"
	)

	###############################################################################
	## file globbing ##############################################################
	###############################################################################

	# copy dir lists to append different file extensions.
	set(src_cpp_files ${src_dirs_${executable}})
	list(TRANSFORM src_cpp_files APPEND "/*.cpp")

	set(src_c_files ${src_dirs_${executable}})
	list(TRANSFORM src_c_files APPEND "/*.c")

	# globbing is more generic, but files can also be listed explicitly.
	file(GLOB_RECURSE srcs_${executable} ${src_cpp_files} ${src_c_files})

	###############################################################################
	## target definitions #########################################################
	###############################################################################

	# select whether this project will build an executable or a library.
	add_executable("${executable}")
	
	# specific configurations for targets.
	if(NOT CMAKE_CROSSCOMPILING)
		add_custom_target("run_${executable}" COMMAND "${executable}" VERBATIM)
	else()
		add_custom_target("run_${executable}"
			COMMAND echo "[INFO] ${executable} is cross-compiled!" VERBATIM
		)
	endif()
	
	# set executable output directories.
	set(build_dir "${PROJECT_BINARY_DIR}/outputs/${CMAKE_LIBRARY_ARCHITECTURE}/binary")
	set(install_dir "${PROJECT_BINARY_DIR}/outputs/install/${CMAKE_LIBRARY_ARCHITECTURE}")
	
	set_target_properties("${executable}" PROPERTIES
		# OUTPUT_NAME "${executable}" # custom name can be given.
		SUFFIX ".elf"
		RUNTIME_OUTPUT_DIRECTORY "${build_dir}"
	)

	# add sources to project target.
	target_sources("${executable}" PRIVATE ${srcs_${executable}})

	# build interface requires full include path.
	set(inc_dirs_full ${inc_dirs_${executable}})
	list(TRANSFORM inc_dirs_full PREPEND "${PROJECT_SOURCE_DIR}/")

	# allow file inclusion relative to root of the src directory with a <> pair.
	target_include_directories("${executable}"
		PUBLIC "$<BUILD_INTERFACE:${inc_dirs_full}>"
		PUBLIC "$<INSTALL_INTERFACE:${inc_dirs_${executable}}>"
	)

	# compiler definitions.
	target_compile_definitions("${executable}"
		PRIVATE PRJ_NAME="${PROJECT_NAME}"
	)

	# target compiler options.
	target_compile_options("${executable}"
		PUBLIC -std=c++17 -Wall -Wextra -Wconversion -Wfloat-equal
	)

	# enable sanitizers.
	if(("${debug_level}" GREATER_EQUAL 3) AND ("${optimization_level}" EQUAL 0) AND (NOT ${CMAKE_CROSSCOMPILING}))
		message(NOTICE "-- Enabling sanitizers...")
		target_link_libraries(${executable}
			PRIVATE -fsanitize=address
			PRIVATE -fsanitize=undefined
			# PRIVATE -fsanitize=leak
			PRIVATE -fsanitize=signed-integer-overflow
			PRIVATE -fsanitize=bounds
		)
	endif()

	###############################################################################
	## dependencies ###############################################################
	###############################################################################

	include(FetchContent)

	target_link_libraries("${executable}" PRIVATE "${library}")

	###############################################################################
	## installation ###############################################################
	###############################################################################

	# all install commands use the same destination.
	# this allows for using paths relative to the executable.
	install(TARGETS "${executable}"
		EXPORT "${executable}Config"
		DESTINATION "${install_dir}"
	)

	# export is useful for including the package from other cmake projects.
	install(EXPORT "${executable}Config"
		DESTINATION "${install_dir}/cmake"
	)
endforeach()

# build proto generated files as library

# set project include directories.
list(APPEND inc_dirs_${library}
	"Protocol/Generated"
)

# set project include directories.
list(APPEND proto_dirs_${library}
	"Protocol/Proto"
)

set(proto_files ${proto_dirs_${library}})
list(TRANSFORM proto_files APPEND "/*.proto")

file(GLOB_RECURSE protos_${library} ${proto_files})

add_library("${library}")

# set library output directories.
set(build_dir "${PROJECT_BINARY_DIR}/outputs/${CMAKE_LIBRARY_ARCHITECTURE}/lib")
set(install_dir "${PROJECT_BINARY_DIR}/outputs/install/lib/${CMAKE_LIBRARY_ARCHITECTURE}/${PROJECT_NAME}")

set_target_properties("${library}" PROPERTIES
	OUTPUT_NAME "${library}_${CMAKE_LIBRARY_ARCHITECTURE}" # custom name can be given.
	PREFIX "lib"
	ARCHIVE_OUTPUT_DIRECTORY "${build_dir}"
	LIBRARY_OUTPUT_DIRECTORY "${build_dir}"
)

# add sources to project target.
target_sources("${library}" PRIVATE ${protos_${library}})

# build interface requires full include path.
set(inc_dirs_full ${inc_dirs_${library}})
list(TRANSFORM inc_dirs_full PREPEND "${PROJECT_SOURCE_DIR}/")

# allow file inclusion relative to root of the src directory with a <> pair.
target_include_directories("${library}"
	PUBLIC "$<BUILD_INTERFACE:${inc_dirs_full}>"
	PUBLIC "$<INSTALL_INTERFACE:${inc_dirs_${library}}>"
)

# target compiler options.
target_compile_options("${library}"
	PUBLIC -std=c++17 -Wall -Wextra -Wconversion -Wfloat-equal
)

# enable sanitizers.
if(("${debug_level}" GREATER_EQUAL 3) AND ("${optimization_level}" EQUAL 0) AND (NOT ${CMAKE_CROSSCOMPILING}))
	message(NOTICE "-- Enabling sanitizers...")
	target_link_libraries(${library}
		PRIVATE -fsanitize=address
		PRIVATE -fsanitize=undefined
		# PRIVATE -fsanitize=leak
		PRIVATE -fsanitize=signed-integer-overflow
		PRIVATE -fsanitize=bounds
	)
endif()

# Find required protobuf package
find_package(protobuf CONFIG REQUIRED)
target_link_libraries(${library} PUBLIC protobuf::libprotobuf)
protobuf_generate(
	TARGET ${library}
	PROTOS ${protos_${library}}
	IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/Protocol/Proto"
	PROTOC_OUT_DIR "${CMAKE_CURRENT_LIST_DIR}/Protocol/Generated"
)

###############################################################################
## custom targets #############################################################
###############################################################################

if(PROJECT_IS_TOP_LEVEL OR NOT has_parent_dir)
	add_custom_target(graphviz_${PROJECT_NAME} ALL
		COMMAND ${CMAKE_COMMAND} "--graphviz=${PROJECT_BINARY_DIR}/dep_graphs/dots/deps.dot" "${PROJECT_SOURCE_DIR}"
		COMMAND bash -c "dot -Tsvg -O $(find ${PROJECT_BINARY_DIR}/dep_graphs/dots/ -type f ! -name *.svg)" VERBATIM
		COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/dep_graphs/svgs"
		COMMAND bash -c "mv ${PROJECT_BINARY_DIR}/dep_graphs/dots/*.svg ${PROJECT_BINARY_DIR}/dep_graphs/svgs/" VERBATIM
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
	)
endif()
