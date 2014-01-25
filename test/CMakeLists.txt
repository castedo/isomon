cmake_minimum_required(VERSION 2.8.1)
project(test-isomon)

add_definitions(-g -Wall -Wno-deprecated -O3)
#add_definitions(-g -Wall -Wno-deprecated -O0 -fno-inline -fno-eliminate-unused-debug-types)

include_directories(..)
add_executable(test-isomon test-isomon.cpp ../currency_data.c)
target_link_libraries(test-isomon boost_unit_test_framework)

