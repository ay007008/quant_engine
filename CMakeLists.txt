cmake_minimum_required(VERSION 3.10)
project(QuantEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags — warnings and optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Build the order book library
add_library(order_book src/order_book.cpp)
target_include_directories(order_book PUBLIC include)

# Build and link the test executable
add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book order_book ${GTEST_LIBRARIES} pthread)
