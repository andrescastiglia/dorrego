cmake_minimum_required(VERSION 3.10)

project(dorrego VERSION 1.0)

add_executable(dorrego main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#configure_file(main.h)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

target_link_libraries(dorrego ${MPI_LIBRARIES})