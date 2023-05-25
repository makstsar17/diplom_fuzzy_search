project(similarityTable)

find_package(RapidJSON CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_library(similarityTable simtable.h simtable.cpp tools.h tools.cpp)

target_link_libraries(similarityTable PRIVATE rapidjson)
target_link_libraries(similarityTable PUBLIC GTest::gtest GTest::gtest_main)