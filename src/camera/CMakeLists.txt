#include headers from this folder
include_directories(${PhotoManager_SOURCE_DIR}/include)

# source files
set(CAMERA_SOURCES "main.cc")

# Build Library
add_executable(camera ${CAMERA_SOURCES})
target_link_libraries(camera ${GLIBS} )           
set_target_properties(camera PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin) 

