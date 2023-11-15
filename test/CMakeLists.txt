cmake_minimum_required(VERSION 3.14)

project(lazyqTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(lazyq REQUIRED)
  enable_testing()
endif()

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(lazyq_test source/lazyq_test.cpp)
target_link_libraries(
    lazyq_test PRIVATE
    lazyq::lazyq
    Catch2::Catch2WithMain
)
target_compile_features(lazyq_test PRIVATE cxx_std_17)

catch_discover_tests(lazyq_test)

# ---- End-of-file commands ----

add_folders(Test)
