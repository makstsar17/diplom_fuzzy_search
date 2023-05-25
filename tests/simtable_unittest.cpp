

#include <random>
#include "gtest/gtest.h"
#include "table_fixture.cpp"


TEST_F(TableFixture, Table){
    std::random_device rd;
    std::mt19937  generator(rd());
    std::uniform_int_distribution<int> distribution(0,  st_->table.size()-1);

    std::vector<size_t> keys;
    for(size_t i = 0; i < 30; i++){
        int ind = distribution(generator);
        keys.push_back(ind);
    }
    size_t iter = 0;
    for(const auto& pair : st_->table ){
        if (std::find(keys.begin(), keys.end(), iter) != keys.end()){
            for(const auto& obj: pair.second){
                for(auto ind : *obj){
                    EXPECT_NE(st_->data[ind].find(pair.first), st_->data[ind].end());
                }
            }
        }
        iter++;
    }
}

TEST_F(TableFixture, getReplaceCost){
    // TEST SYMBOL A
    EXPECT_EQ(st_->getReplaceCost(L'a', L'q').value(), 0.75);
    EXPECT_EQ(st_->getReplaceCost(L'a', L'A').value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'a', L'l'), std::nullopt);
    EXPECT_EQ(st_->getReplaceCost(L'a', L'\u01fb').value(), 0.5);
    auto testAO = st_->getReplaceCost(L'a', L'o').value();
    EXPECT_TRUE(0.84 < testAO and testAO < 0.86);

    // TEST SYMBOL G
    EXPECT_EQ(st_->getReplaceCost(L'g', L't').value(), 0.75);
    EXPECT_EQ(st_->getReplaceCost(L'g', L'G').value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'g', L'z'), std::nullopt);
    EXPECT_EQ(st_->getReplaceCost(L'g', L'\u0123').value(), 0.5);
    auto testGQ = st_->getReplaceCost(L'g', L'q').value();
    EXPECT_TRUE(0.84 < testGQ and testGQ < 0.86);

    // TEST SYMBOL N
    EXPECT_EQ(st_->getReplaceCost(L'n', L'h').value(), 0.75);
    EXPECT_EQ(st_->getReplaceCost(L'n', L'N').value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'n', L'w'), std::nullopt);
    EXPECT_EQ(st_->getReplaceCost(L'n', L'\u00f1').value(), 0.5);
    auto testNM = st_->getReplaceCost(L'n', L'v').value();
    EXPECT_TRUE(0.84 < testNM and testNM < 0.86);

    // TEST SYMBOL R
    EXPECT_EQ(st_->getReplaceCost(L'r', L'e').value(), 0.75);
    EXPECT_EQ(st_->getReplaceCost(L'r', L'R').value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'r', L'x'), std::nullopt);
    EXPECT_EQ(st_->getReplaceCost(L'r', L'\u0157').value(), 0.5);
    auto testRV = st_->getReplaceCost(L'r', L'v').value();
    EXPECT_TRUE(0.84 < testRV and testRV < 0.86);
}