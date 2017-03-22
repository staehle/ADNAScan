cmake_minimum_required(VERSION 2.8.10)
project(adna)

set(adna_VERSION_MAJOR 0)
set(adna_VERSION_MINOR 1)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        message(FATAL_ERROR "GCC version must be at least 4.8!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with GCC 4.8+")
endif()

add_definitions(-std=gnu++11)
#target_compile_definitions(adna PROPERTIES COMPILE_FLAGS -Wnoerror=)

set(CMAKE_BUILD_TYPE Debug)

find_package(MPI REQUIRED)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.58.0 COMPONENTS mpi serialization) 
if(Boost_FOUND)
  if(MPI_FOUND)	
    include_directories(${Boost_INCLUDE_DIRS} ${MPI_INCLUDE_PATH}) 
    add_executable(mpi-adna adna.cpp StringTinder.cpp) 
    target_link_libraries(mpi-adna ${Boost_LIBRARIES} ${MPI_LIBRARIES})
  endif()
endif()

add_executable(adna run.cpp)

