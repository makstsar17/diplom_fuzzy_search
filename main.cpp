//
// Created by makstsar on 10.05.2023.
//

#include <random>
#include <fstream>
#include <locale>
#include <codecvt>

#include <iostream>
#include "src/simtable.h"
#include "src/tools.h"

bool test(const SimTable& table);
bool check(const std::wstring& a, const std::wstring& b, const SimTable& table, float res);

int main() {
    const std::string pathToFile = "..\\simtable.json";
    try {
        SimTable table(pathToFile);
        if (!test(table)) std::cout << "Error" << std::endl;
        {
            std::cout << "Complete." << std::endl;
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() <<std::endl;
        return -1;
    }
    return 0;
}


bool test(const SimTable& table)
{
    if (!check(L"aba", L"aca", table, 1.0f)) return false;
    if (!check(L"aba", L"baca", table, 2.0f)) return false;
    if (!check(L"bac", L"bca", table, 1.0f)) return false;

    return true;
}

bool check(const std::wstring& a, const std::wstring& b, const SimTable& table, float res)
{
    return editDistance(a, b) == res;
    // return editDistance(a, b, table) == res;
}