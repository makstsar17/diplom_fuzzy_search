
#ifndef DIPLOM_LAST_SIMTABLE_H
#define DIPLOM_LAST_SIMTABLE_H

#include <optional>
#include <map>
#include <memory>
#include <vector>
#include <set>
#include <string>
#include "gtest/gtest.h"

struct SimObject{
    explicit SimObject(float price);
    void insert(int value);
    std::vector<int>::iterator begin();
    std::vector<int>::iterator end();
    float getPrice() const;
    size_t getSize() const;
private:
    float price;
    std::vector<int> listOfSimCharGroupInd;
};

class SimTable
{
private:
    std::map<wchar_t, std::vector<std::shared_ptr<SimObject>>> table;
    std::vector<std::set<wchar_t>> data;
    std::map<std::string, float> cost;
public:
    explicit SimTable(const std::string& path);
    ~SimTable() = default;

    SimTable(const SimTable&) = delete;
    SimTable &operator=(const SimTable&) = delete;
    SimTable(SimTable&&) = delete;
    SimTable &operator=(SimTable&&) = delete;

    std::optional<float> getReplaceCost(wchar_t c1, wchar_t c2) const;
    wchar_t getRandomSimSymbol(wchar_t wch);


    FRIEND_TEST(TableFixture, Table);
    FRIEND_TEST(TableFixture, editDistanceWithTable);
};


#endif //DIPLOM_LAST_SIMTABLE_H
