
#include "simtable.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cwctype>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/istreamwrapper.h"
#include <random>


SimObject::SimObject(float price) : price(price)
{}

void SimObject::insert(int value){
    SimObject::listOfSimCharGroupInd.push_back(value);
}

std::vector<int>::iterator SimObject::begin(){
    return SimObject::listOfSimCharGroupInd.begin();
}

std::vector<int>::iterator SimObject::end(){
    return SimObject::listOfSimCharGroupInd.end();
}

float SimObject::getPrice() const {
    return SimObject::price;
}

size_t SimObject::getSize() const {
    return SimObject::listOfSimCharGroupInd.size();
}

typedef rapidjson::GenericDocument<rapidjson::UTF16LE<> > WDocument;

SimTable::SimTable(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    if (ifs.fail()) {
        throw std::ifstream::failure("Could not open file: " + std::string(path));
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::AutoUTFInputStream<unsigned , rapidjson::IStreamWrapper> eis(isw);

    WDocument document;
    document.ParseStream<0, rapidjson::AutoUTF<unsigned> >(eis);
    if (!document.IsObject()){
        throw std::runtime_error("Opened file is not JSON");
    }

    //PARSE DATA
    for (auto key = document.MemberBegin(); key != document.MemberEnd(); key++){
        if(key->name == L"data"){
            for(auto row = key->value.Begin(); row != key->value.End(); row++){
                std::set<wchar_t> set = {};
                auto array = row->GetArray();
                for (size_t ind = 0; ind < array.Size(); ind++){
                    set.insert(array[ind].GetString()[0]);
                }
                data.push_back(set);
            }
            break;
        }
    }

    //PARSE SIMTABLE
    for (auto key = document.MemberBegin(); key != document.MemberEnd(); key++) {
        if(key->value.IsObject() and key->name != L"cost"){
            for (auto keyPrice = key->value.MemberBegin(); keyPrice != key->value.MemberEnd(); keyPrice++){
                try {
                    const auto price = std::stof(keyPrice->name.GetString());
                    auto simObject = std::make_shared<SimObject>(price);
                    for (size_t ind = 0; ind < keyPrice->value.Size(); ind++){
                        simObject->insert(keyPrice->value[ind].GetInt());
                    }
                    table[key->name.GetString()[0]].push_back(simObject);

                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() <<std::endl;
                }
            }
        }
    }

    //PARSE COST
    for (auto key = document.MemberBegin(); key != document.MemberEnd(); key++) {
        if (key->name == L"cost") {
            for(auto costName = key->value.MemberBegin(); costName != key->value.MemberEnd(); costName++){
                std::wstring wstr = costName->name.GetString();
                std::string str(wstr.begin(), wstr.end());
                cost[str] = costName->value.GetFloat();
            }
        }
    }
}

std::optional<float> SimTable::getReplaceCost(wchar_t c1, wchar_t c2) const
{

    std::optional<float> result;

    bool isUpperC1 = std::iswupper(c1);
    bool isUpperC2 = std::iswupper(c2);


    wchar_t lowerC1 = isUpperC1 ? std::towlower(c1) : c1;
    wchar_t lowerC2 = isUpperC2 ? std::towlower(c2) : c2;
    if (lowerC1 == lowerC2){
        return cost.at("CAPITAL_COST");
    }

    auto simObjs = table.find(lowerC1);
    if (simObjs != table.end()) {
        for (auto &simObj: table.find(lowerC1)->second) {
            for (auto dataInd = simObj->begin(); dataInd != simObj->end(); dataInd++) {
                if (data[*dataInd].find(lowerC2) != data[*dataInd].end()) {
                    if (!result.has_value() or result > simObj->getPrice())
                        result = simObj->getPrice();
                }
            }
        }
    }

    if ((isUpperC1 xor isUpperC2) and result.has_value())
        result.emplace(result.value() + cost.at("CAPITAL_COST"));

    return result;
}

wchar_t SimTable::getRandomSimSymbol(wchar_t wch) {
    std::random_device rd;
    std::mt19937  generator(rd());

    auto simObjs = table.find(wch);
    if (simObjs != table.end()) {
        std::uniform_int_distribution<int> distSimObj(0, simObjs->second.size() - 1);
        auto randObj = distSimObj(generator);
        auto simObj = simObjs->second[randObj];
        while (simObj->getSize() == 0){
            randObj = distSimObj(generator);
            simObj = simObjs->second[randObj];
        }

        std::uniform_int_distribution<int> distSimGroup(0, simObj->getSize() - 1);
        auto randGroup = distSimGroup(generator);

        auto it = simObj->begin();
        std::advance(it, randGroup);
        size_t dataInd = *it;

        std::set<wchar_t> simGroup (data[dataInd]);
        simGroup.erase(wch);
        std::uniform_int_distribution<int> distSimChar(0, simGroup.size() - 1);
        auto randChar = distSimChar(generator);
        size_t iter = 0;
        for(auto c : simGroup){
            if (iter == randChar){
                return c;
            }
            iter++;
        }
    }
    return L'\0';

}
