#include headers from this folder
include_directories(${PhotoManager_SOURCE_DIR}/include)

# source files
set(DAEMON_SOURCES "daemon.cc")

# Build Library
add_executable(daemon ${DAEMON_SOURCES})
target_link_libraries(daemon ${GLIBS} )           
set_target_properties(daemon PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)   

