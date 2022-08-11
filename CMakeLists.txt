cmake_minimum_required(VERSION 3.23)
project(kmeans)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(domain)

add_executable(kmeans main.cpp)
target_link_libraries(kmeans domain)
