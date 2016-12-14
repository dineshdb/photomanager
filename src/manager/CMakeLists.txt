#include headers from this folder
include_directories(${PhotoManager_SOURCE_DIR}/include)

# source files
set(MANAGER_SOURCES "main.cc")

# Build Library
add_executable(manager ${MANAGER_SOURCES})
target_link_libraries(manager ${GLIBS} ${OpenCV_LIBS})
set_target_properties(manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin) 

