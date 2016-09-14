cmake_minimum_required(VERSION 2.8)
project(adna)

set(adna_VERSION_MAJOR 0)
set(adna_VERSION_MINOR 1)

add_executable(adna-init init.cpp)
add_executable(adna-gompi mpiworld.cpp ReadPair.cpp)
add_executable(adna-check checker.cpp)

add_definitions(-std=gnu++0x)

find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})

target_link_libraries(adna-init rt)
target_link_libraries(adna-gompi rt)
target_link_libraries(adna-gompi ${MPI_LIBRARIES})
target_link_libraries(adna-check rt)
target_link_libraries(adna-check curses)

