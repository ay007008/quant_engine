cmake_minimum_required(VERSION 3.10)
project(QuantEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(order_book src/order_book.cpp)
target_include_directories(order_book PUBLIC include)

add_library(matching_engine src/matching_engine.cpp)
target_include_directories(matching_engine PUBLIC include)
target_link_libraries(matching_engine order_book)

add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book order_book ${GTEST_LIBRARIES} pthread)

add_executable(test_matching_engine tests/test_matching_engine.cpp)
target_link_libraries(test_matching_engine matching_engine ${GTEST_LIBRARIES} pthread)
