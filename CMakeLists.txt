cmake_minimum_required(VERSION 3.0)
project(PhotoManager)

SET(CMAKE_CXX_COMPILER "g++")
# Respect CXX Flags set outside
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

include_directories(include)

# various project names
SET(PROJECT_NAMES "camera" "daemon" "manager")

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(docs
		${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doxy.config
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		DEPENDS doxy.config
	)
endif(DOXYGEN_FOUND)

foreach(PROJECT_NAME ${PROJECT_NAMES})
  # add sub-directories
  add_subdirectory("${CMAKE_SOURCE_DIR}/src/${PROJECT_NAME}"
    "${CMAKE_CURRENT_BINARY_DIR}/out/${PROJECT_NAME}")

endforeach(PROJECT_NAME ${PROJECT_NAMES})
