cmake_minimum_required(VERSION 2.6)

project(eset-challenge)
  
add_executable(eset-challenge 
				src/main.cpp
)

install(TARGETS eset-challenge 
		DESTINATION bin)
