cmake_minimum_required(VERSION 3.14)
project(extra_test_setlib)

file(GLOB SOURCES "**/*.cpp")

find_package(GTest REQUIRED)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -lgcov" )

add_executable(${PROJECT_NAME} ${SOURCES} ${SETLIB_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SETLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} setlib)
