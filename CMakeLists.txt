cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(integration_test VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Find Xacc
find_package(XACC REQUIRED)

add_executable(integration_test integration_test.cpp)
target_link_libraries(integration_test PRIVATE xacc::xacc)