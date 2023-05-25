

#include "gtest/gtest.h"
#include "../src/simtable.h"

class TableFixture : public testing::Test {
protected:
    void SetUp() override {
        const std::string pathToFile = "..\\..\\simtable.json";
        st_ = new SimTable(pathToFile);
    }
    SimTable* st_;
};
