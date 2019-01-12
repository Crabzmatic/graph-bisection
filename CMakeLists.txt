cmake_minimum_required(VERSION 3.10)
project(graph_bisection)

set(CMAKE_CXX_STANDARD 17)
set(DBGFLAGS "-g -O0 -fconcepts -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ${DBGFLAGS}")
set(BA_GRAPH "-DBA_GRAPH_DEBUG -I ../ba-graph/include/")

set(SOURCE_FILES main.cpp)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)
