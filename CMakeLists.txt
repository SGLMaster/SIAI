cmake_minimum_required (VERSION 2.6)
project (SIAI)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)

set(WXWIDGETS_HOME C:/wxWidgets)

include_directories(${WXWIDGETS_HOME}/include ${WXWIDGETS_HOME}/lib/gcc_dll/mswud include/siai)

link_directories(${WXWIDGETS_HOME}/lib/gcc_dll)

file(GLOB_RECURSE EDITOR_SOURCES "src/siai/editor/*.cpp")
file(GLOB_RECURSE MAP_SOURCES "src/siai/map/*.cpp")
file(GLOB PAINTER_SOURCES "src/siai/painter/*.cpp")
file(GLOB UTIL_SOURCES "src/siai/util/*.cpp")
file(GLOB ROOT_SOURCES "src/siai/*.cpp")

#Map Library
add_library(siaimap SHARED ${MAP_SOURCES} ${PAINTER_SOURCES} ${UTIL_SOURCES} ${ROOT_SOURCES})

target_link_libraries(siaimap wxbase30ud wxmsw30ud_core user32)
target_compile_options(siaimap PUBLIC -std=c++14 -m64 -Wall -Wno-deprecated-declarations -g -DBUILD_DLL)
	
#Map Editor
add_executable(map-editor ${EDITOR_SOURCES} ${PAINTER_SOURCES} ${UTIL_SOURCES} ${ROOT_SOURCES})

target_link_libraries(map-editor siaimap wxbase30ud wxmsw30ud_core wxmsw30ud_adv)
target_compile_options(map-editor PUBLIC -std=c++14 -m64 -Wall -Wno-deprecated-declarations -g)
target_link_options(map-editor PUBLIC -mwindows)