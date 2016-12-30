#include headers from this folder
include_directories(${PhotoManager_SOURCE_DIR}/include)

# source files
set(MANAGER_SOURCES "main.cc")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM-2.4 REQUIRED glibmm-2.4 giomm-2.4)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GLIBMM_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

link_directories(${GTKMM_LIBRARY_DIRS})

# Build Library
add_executable(manager ${MANAGER_SOURCES})

target_link_libraries(manager -pthread -lrt ${GLIBS} ${GTKMM_LIBRARIES})

set_target_properties(manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

install(TARGETS manager DESTINATION /usr/local/bin)

