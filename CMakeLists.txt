cmake_minimum_required (VERSION 2.6)
project (SIAI)

set(WXWIDGETS_HOME C:/wxWidgets)
set(MYSQLPP_HOME C:/MySQL++)
set(CONNECTOR_C_HOME C:/ConnectorC)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)

include_directories(${WXWIDGETS_HOME}/include ${WXWIDGETS_HOME}/lib/gcc_dll/mswud ${MYSQLPP_HOME}/include ${CONNECTOR_C_HOME}/include include/siai)

link_directories(${WXWIDGETS_HOME}/lib/gcc_dll ${MYSQLPP_HOME}/lib/Debug ${CONNECTOR_C_HOME}/lib)

file(GLOB DATABASE_SOURCES "src/siai/database/*.cpp")
file(GLOB_RECURSE MANAGER_SOURCES "src/siai/manager/*.cpp")
file(GLOB_RECURSE MAP_SOURCES "src/siai/map/*.cpp")
file(GLOB SERVER_SOURCES "src/siai/server/*.cpp")
file(GLOB PAINTER_SOURCES "src/siai/painter/*.cpp")
file(GLOB NETWORK_SOURCES "src/siai/network/*.cpp")
file(GLOB UTIL_SOURCES "src/siai/util/*.cpp")
file(GLOB ROOT_SOURCES "src/siai/*.cpp")

set(CMAKE_CXX_FLAGS "-std=c++14 -m64 -Wall -Wno-deprecated-declarations -Wno-attributes")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

#Map Library
add_library(siaimap SHARED ${MAP_SOURCES} ${DATABASE_SOURCES} ${PAINTER_SOURCES} ${NETWORK_SOURCES} ${UTIL_SOURCES} ${ROOT_SOURCES})

target_link_libraries(siaimap wxbase30ud wxmsw30ud_core wxbase30ud_net mysqlpp mysql user32)
target_compile_options(siaimap PUBLIC -DBUILD_DLL)
	
#Map Editor
add_executable(manager ${MANAGER_SOURCES} ${DATABASE_SOURCES} ${PAINTER_SOURCES} ${UTIL_SOURCES} ${ROOT_SOURCES})

target_link_libraries(manager siaimap wxbase30ud wxmsw30ud_core wxmsw30ud_adv mysqlpp mysql)
target_link_options(manager PUBLIC -mwindows)

#Server
add_executable(server ${SERVER_SOURCES} ${DATABASE_SOURCES})

target_link_libraries(server siaimap wxbase30ud mysqlpp mysql)