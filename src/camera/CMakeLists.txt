#include headers from this folder
include_directories(${PhotoManager_SOURCE_DIR}/include)

# source files
set(CAMERA_SOURCES "main.cc")

find_package(PkgConfig REQUIRED)
pkg_check_modules (GLIBMM-2.4 REQUIRED glibmm-2.4 giomm-2.4)
pkg_check_modules (GTKMM REQUIRED gtkmm-3.0)

include_directories(${GLIBMM_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

link_directories(${GTKMM_LIBRARY_DIRS}  )

# Build Library
add_executable(camera ${CAMERA_SOURCES})
target_link_libraries(camera
	-pthread
	-lrt
	${GLIBS}
	${GTKMM_LIBRARIES}
)
set_target_properties(camera PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
install(TARGETS camera DESTINATION /usr/local/bin)

