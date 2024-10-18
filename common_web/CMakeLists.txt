project(common_web)
cmake_minimum_required(VERSION 2.8)
MESSAGE(STATUS "#########################")
ADD_DEFINITIONS(-std=c++11   -fpermissive )

file(GLOB ALL_CXX_SRCS
        ./*.cpp  ./*.cc
        ./*.h
		./*.hpp
        ./http_message/*.cc
        ./http_message/*.h
        ./http_message/*.cpp
        )

add_library(${PROJECT_NAME} ${ALL_CXX_SRCS})
target_link_libraries(${PROJECT_NAME} )
