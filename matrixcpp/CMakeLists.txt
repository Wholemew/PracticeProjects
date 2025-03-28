cmake_minimum_required(VERSION 3.14)
project(s21_matrix_oop)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Werror -Wextra")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  s21_matrix_tests
  s21_matrix_tests.cpp
  s21_matrix_oop.cpp
)
target_link_libraries(
  s21_matrix_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(s21_matrix_tests)
