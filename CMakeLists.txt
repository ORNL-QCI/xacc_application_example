cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(xacc_app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(XACC REQUIRED)

add_executable(xacc_app xacc_app.cpp)
target_link_libraries(xacc_app PRIVATE xacc::xacc)
