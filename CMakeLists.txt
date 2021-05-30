cmake_minimum_required(VERSION 3.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")



set(CXX_FLAGS "-Wall -O3")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
project(wiki-indexer)

add_executable(wiki-indexer Source.cpp Indexer.h Indexer.cpp)

