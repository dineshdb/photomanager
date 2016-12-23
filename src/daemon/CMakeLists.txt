#include headers from this folder


# source files
set(DAEMON_SOURCES "daemon.cc" "cmd-parser.cc" "dbus-service.cc" "recognizer.cc")

find_package(OpenCV REQUIRED)

set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK ON)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

find_package(PkgConfig REQUIRED)
pkg_check_modules (GLIBMM-2.4 REQUIRED glibmm-2.4 giomm-2.4)
pkg_check_modules (GTKMM REQUIRED gtkmm-3.0)

include_directories(${GLIBMM_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

link_directories(${GTKMM_LIBRARY_DIRS}  )

if (Boost_FOUND)
  # From the offical documentation:
  # Add include directories to the build. [...] If the SYSTEM option is given,
  # the compiler will be told the directories are meant as system include
  # directories on some platforms (signalling this setting might achieve effects
  # such as the compiler skipping warnings [...])."
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})

  # From the offical documentation:
  # "Specify directories in which the linker will look for libraries. [...] Note
  # that this command is rarely necessary. Library locations returned by
  # find_package() and find_library() are absolute paths. Pass these absolute
  # library file paths directly to the target_link_libraries() command. CMake
  # will ensure the linker finds them."
  #link_directories (${Boost_LIBRARY_DIRS})
endif ()

include_directories(${PhotoManager_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
# Build Library
add_executable(daemon ${DAEMON_SOURCES})

target_link_libraries(daemon
	-pthread
	-lrt
	${GLIBS}
	${Boost_LIBRARIES}
	${OpenCV_LIBS}
	${GTKMM_LIBRARIES}
)
set_target_properties(daemon PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)   

