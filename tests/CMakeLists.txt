project(tests)

find_package(GTest CONFIG REQUIRED)

add_executable(tests editdistance_unittest.cpp simtable_unittest.cpp table_fixture.cpp)

target_link_libraries(tests PUBLIC similarityTable GTest::gtest GTest::gtest_main)

