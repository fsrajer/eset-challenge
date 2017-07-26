cmake_minimum_required(VERSION 2.6)

project(eset-challenge)

# enable visual studio folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

# Find Catch, the test lib
include(FindPackageHandleStandardArgs)
find_path(CATCH_INCLUDE_DIR catch.hpp HINTS "../catch")
find_package_handle_standard_args(CATCH DEFAULT_MSG CATCH_INCLUDE_DIR)

# Create an interface in order for linking Catch to tests
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CATCH_INCLUDE_DIR})

# EC as in Eset Challenge
macro(EC_ADD_TEST TARGET_NAME)
    # ${ARGN} will store the list of source files passed to this function.
    add_executable(${TARGET_NAME} ${ARGN})
	target_link_libraries(${TARGET_NAME} catch)
	set_target_properties(${TARGET_NAME} PROPERTIES FOLDER tests)
    add_test("tests/${TARGET_NAME}" ${TARGET_NAME})
    install(TARGETS ${TARGET_NAME} 
			DESTINATION test)
endmacro(EC_ADD_TEST)
  
add_executable(eset-challenge 
				src/main.cpp
				src/pattern_search.h 
				src/pattern_search.cpp
)

EC_ADD_TEST(dummy_test src/dummy_test.cpp)

install(TARGETS eset-challenge 
		DESTINATION bin)
