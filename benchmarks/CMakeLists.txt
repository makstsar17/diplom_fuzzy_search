project(benchmarks)

find_package(benchmark REQUIRED)

add_executable(benchmarks fuzzy_search_benchmark.cpp)

target_link_libraries(benchmarks similarityTable benchmark::benchmark)
