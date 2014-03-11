cmake_minimum_required(VERSION 2.8.1)
project(test-isomon)

find_package(Boost 1.49.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})
message(${Boost_LIBRARIES})

add_definitions(-g -Wall -Wno-deprecated -O3)
#add_definitions(-g -Wall -Wno-deprecated -O0 -fno-inline -fno-eliminate-unused-debug-types)
include_directories(..)

add_executable(test-isomon
  test-isomon.cpp
  test-money_calc.cpp
  ../currency_data.c)
target_link_libraries(test-isomon ${Boost_LIBRARIES})

enable_testing()
add_test(NAME test-isomon COMMAND test-isomon -l message)

