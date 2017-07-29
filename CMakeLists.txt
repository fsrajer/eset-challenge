cmake_minimum_required(VERSION 2.8)

project(eset-challenge)

if(NOT WIN32)
  message(FATAL_ERROR "Windows needed due to the use of windows.h functions for handling directories.")
endif()

# enable visual studio folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Find Catch, the test lib
include(FindPackageHandleStandardArgs)
find_path(CATCH_INCLUDE_DIR catch.hpp HINTS "../catch")
find_package_handle_standard_args(CATCH DEFAULT_MSG CATCH_INCLUDE_DIR)

if(${CATCH_FOUND})
	enable_testing()
	
	# Create an interface in order for linking Catch to tests
	add_library(catch INTERFACE)
	target_include_directories(catch INTERFACE ${CATCH_INCLUDE_DIR})
else()
	message(STATUS "Catch not found. Building without tests.")
endif()


# Main library
add_library(PatternSearch 
			  src/PatternSearch.h 
			  src/PatternSearch.cpp
			  src/Utils.h 
			  src/Utils.cpp 
			  src/FileCrawler.h 
			  src/FileCrawler.cpp 
			  src/TextSegment.h 
			  src/TextSegment.cpp 
			  src/Semaphore.h 
			  src/Semaphore.cpp 
			  src/ProducerConsumerBuffer.h
)

add_executable(PatternSearchApp
			     src/main.cpp)
target_link_libraries(PatternSearchApp PatternSearch)

# EC as in Eset Challenge
macro(EC_ADD_TEST TARGET_NAME)
    # ${ARGN} will store the list of source files passed to this function.
    add_executable(${TARGET_NAME} ${ARGN})
	target_link_libraries(${TARGET_NAME} catch PatternSearch)
	set_target_properties(${TARGET_NAME} PROPERTIES FOLDER tests)
    add_test("tests/${TARGET_NAME}" ${TARGET_NAME})
    install(TARGETS ${TARGET_NAME} 
			DESTINATION test)
endmacro(EC_ADD_TEST)
  
if(${CATCH_FOUND})
	EC_ADD_TEST(PatternSearchTest 
				src/PatternSearchTest.cpp)
	EC_ADD_TEST(TextSegmentTest 
				src/TextSegmentTest.cpp)
	EC_ADD_TEST(UtilsTest 
				src/UtilsTest.cpp)
endif()

install(TARGETS PatternSearch 
		DESTINATION lib)
install(TARGETS PatternSearchApp
		DESTINATION bin)
