project(clib)
cmake_minimum_required(VERSION 2.8)
MESSAGE(STATUS "#########################")
ADD_DEFINITIONS(-std=c++11   -fpermissive )

file(GLOB ALL_CXX_SRCS
        ./*.cpp  ./*.cc
        ./*.h
        )

add_library(${PROJECT_NAME} ${ALL_CXX_SRCS})
target_link_libraries(${PROJECT_NAME} )
