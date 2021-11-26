cmake_minimum_required(VERSION 3.0.0)
project(setlib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g --coverage -O0 -Werror -Wall")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage -shared -lgcov" )

set(SETLIB_INCLUDE_DIRS ${SETLIB_INCLUDE_DIRS} ${CMAKE_HOME_DIRECTORY}/include/)
set(SETLIB_HEADERS ${SETLIB_HEADERS} ${CMAKE_HOME_DIRECTORY}/include/set.hpp)

add_library(${PROJECT_NAME} STATIC ${SETLIB_HEADERS})
set_target_properties(setlib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC ${SETLIB_INCLUDE_DIRS})
add_subdirectory(tests)

add_subdirectory(tests/extra_test)
add_subdirectory(tests/speed_test)

