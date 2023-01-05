cmake_minimum_required(VERSION 3.22)

project(dorrego VERSION 0.1.0)

set(PROJECT_VERSION_META "-alpha")
set(VERSION_REGEX "#define DR_VERSION[ \t]+\"(.+)\"")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

configure_file(main.h.in main.h)
include_directories(build)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH}) 
add_definitions(${MPI_DEFINITIONS})
link_directories(${MPI_LIBRARY_DIRS})

find_package(LLVM REQUIRED CONFIG)
add_compile_options(-O3)
add_compile_options(-march=native)
add_compile_options(-mavx2)
add_compile_options(-ffast-math)

find_package(Boost REQUIRED COMPONENTS system mpi)
include_directories(${Boost_INCLUDE_DIRS}) 
add_definitions(${Boost_DEFINITIONS})
link_directories(${Boost_LIBRARY_PATH})

set(BOOST_USE_STATIC_LIBS OFF) 
set(BOOST_USE_MULTITHREADED ON)  
set(BOOST_USE_STATIC_RUNTIME OFF) 

set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

file(GLOB SOURCES *.cpp)
add_executable(dorrego ${SOURCES})

target_link_libraries(dorrego ${Boost_LIBRARIES} ${MPI_LIBRARIES})