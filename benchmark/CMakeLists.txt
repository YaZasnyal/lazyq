cmake_minimum_required(VERSION 3.23)

find_package(benchmark REQUIRED)
add_executable(lazyq_benchmark main.cpp)
target_link_libraries(lazyq_benchmark 
    PRIVATE 
    benchmark::benchmark 
    lazyq::lazyq
)