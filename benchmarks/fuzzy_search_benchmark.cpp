
#include "benchmark/benchmark.h"
#include <fstream>
#include <sstream>
#include <random>
#include <codecvt>
#include <algorithm>
#include "../src/simtable.h"
#include "../src/tools.h"


std::wofstream createWofstream(const std::string& filename){
    std::wofstream file(filename, std::ios::binary | std::ios::trunc);
    const unsigned long MaxCode = 0x10ffff;
    const std::codecvt_mode Mode = std::generate_header;
    std::locale utf16_locale_data(file.getloc(), new std::codecvt_utf16<wchar_t, MaxCode, Mode>);
    file.imbue(utf16_locale_data);
    if (!file.is_open())
        std::cerr << "Failed to create the file: " << filename << std::endl;
    return file;
}

std::wifstream createWifstream(const std::string& filename){
    std::wifstream file(filename, std::ios::binary);
    const unsigned long MaxCode = 0x10ffff;
    const std::codecvt_mode Mode = std::generate_header;
    std::locale utf16_locale_data(file.getloc(), new std::codecvt_utf16<wchar_t, MaxCode, Mode>);
    file.imbue(utf16_locale_data);
    if (!file.is_open())
        std::cerr << "Failed to create the file: " << filename << std::endl;
    return file;
}


class SetUpFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        const std::string pathToFile = "..\\..\\simtable.json";
        st_ = new SimTable(pathToFile);

        createTestFiles(state.range(0), state.range(1));

        std::stringstream ss;
        ss << R"(..\..\benchmarks\temp\request_word)" << state.range(0) << "_" << state.range(1) << ".txt";
        std::wifstream requestFile = createWifstream(ss.str());

        getline(requestFile, requestWord);
        requestFile.close();

        ss.str("");

        ss << R"(..\..\benchmarks\temp\temp_words)" << state.range(0) << "_" << state.range(1) << ".txt";
        text = createWifstream(ss.str());

        ss.str("");
        ss << R"(..\..\benchmarks\result\result_with_table)"<< state.range(0) << "_" << state.range(1) << ".txt";
        resultFile = createWofstream(ss.str());
    }

    void TearDown(const ::benchmark::State& state) override {
        text.close();
        resultFile.close();
    }

protected:
    SimTable* st_;
    std::wstring requestWord;
    std::wifstream text;
    std::wofstream resultFile;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

private:
    void createTestFiles(int arg1, int arg2){
        std::stringstream dataFileNameStream;
        dataFileNameStream << R"(..\..\benchmarks\temp\temp_words)" << arg1 << "_" << arg2 << ".txt";
        std::wofstream dataFile = createWofstream(dataFileNameStream.str());


        std::stringstream requestFileNameStream;
        requestFileNameStream << R"(..\..\benchmarks\temp\request_word)" << arg1 << "_" << arg2 << ".txt";
        std::wofstream requestFile = createWofstream(requestFileNameStream.str());


//        std::ifstream words(R"(..\..\benchmarks\words.txt)");
        std::ifstream words(R"(..\..\benchmarks\words_test.txt)");
        if(!words.is_open())
            std::cerr << "Failed to open the file: " << "words.txt" << std::endl;


        size_t lineCount = 0;
        std::string line;
        while (getline(words, line)) {
            lineCount++;
        }
        words.clear();
        words.seekg(0, std::ios::beg);

        std::random_device rd;
        std::mt19937  generator(rd());

        std::vector<size_t> lineInd;
        for(size_t i = 0; i < lineCount; i++)
            lineInd.push_back(i);
        std::shuffle(lineInd.begin(), lineInd.end(), generator);
        auto wordCountToChange = lineCount * arg1 / 100;
        lineInd.erase(lineInd.begin() + wordCountToChange, lineInd.end());
        std::set<size_t> indForMistake(lineInd.begin(), lineInd.end());

        size_t requestWordInd;
        if(lineInd.size() != 0) {
            std::uniform_int_distribution<int> dist(0, lineInd.size() - 1);
            requestWordInd = lineInd[dist(generator)];
        }
        else{
            std::uniform_int_distribution<int> dist(0,  lineCount - 1);
            requestWordInd = dist(generator);
        }

        size_t iter = 0;
        while (getline(words, line)) {
            std::wstring resWord = converter.from_bytes(line);
            if (indForMistake.find(iter) != indForMistake.end())
                resWord = makeMistake(resWord, st_);

            if(iter == requestWordInd){
                requestFile << resWord;
            }

            dataFile << resWord << converter.from_bytes('\n');
            iter++;
        }
        requestFile.close();
        dataFile.close();
    }
    std::wstring makeMistake(const std::wstring& word, SimTable* table){
        std::random_device rd;
        std::mt19937  generator(rd());
        std::uniform_int_distribution<int> dist(0, word.size() - 1);

        std::set<int> mistakeInd;
        for (size_t i = 0; i < word.size() / 10 + 1; i++){
            mistakeInd.insert(dist(generator));
        }

        auto resWord = word;
        for(auto ind : mistakeInd){
            resWord[ind] = table->getRandomSimSymbol(word[ind]);
        }

        return resWord;
    }
};

class TearDownFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        std::stringstream ss;
        ss << R"(..\..\benchmarks\temp\request_word)" << state.range(0) << "_" << state.range(1) << ".txt";
        std::wifstream requestFile = createWifstream(ss.str());
        getline(requestFile, requestWord);
        requestFile.close();

        ss.str("");

        ss << R"(..\..\benchmarks\temp\temp_words)" << state.range(0) << "_" << state.range(1) << ".txt";
        text = createWifstream(ss.str());

        ss.str("");
        ss << R"(..\..\benchmarks\result\result_without_table)"<< state.range(0) << "_" << state.range(1) << ".txt";
        resultFile = createWofstream(ss.str());
    }

    void TearDown(const ::benchmark::State& state) override {
        resultFile.close();
        text.close();
        std::stringstream FileNameStream;
        FileNameStream << R"(..\..\benchmarks\temp\temp_words)" << state.range(0) << "_" << state.range(1) << ".txt";
        if (std::remove(FileNameStream.str().c_str()) != 0){
            std::cout << "Failed to delete the file: " << FileNameStream.str() << std::endl;
        }
        FileNameStream.str("");
        FileNameStream << R"(..\..\benchmarks\temp\request_word)" << state.range(0) << "_" << state.range(1) << ".txt";
        if (std::remove(FileNameStream.str().c_str()) != 0){
            std::cout << "Failed to delete the file: " << FileNameStream.str() << std::endl;
        }
    }
protected:
    std::wstring requestWord;
    std::wifstream text;
    std::wofstream resultFile;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
};

BENCHMARK_DEFINE_F(SetUpFixture, BenchmarkWithSimTable)(benchmark::State& state){
    for (auto _ : state) {
        state.PauseTiming();
        resultFile << requestWord << converter.from_bytes(" : 0") << converter.from_bytes('\n');
        std::wstring comp;
        size_t countMatch = 0;

        while(getline(text, comp)) {
            state.ResumeTiming();
            auto result = editDistance(requestWord, comp, *st_);
            state.PauseTiming();
            if (result < (state.range(1) + 0.1)) {
                resultFile << comp << converter.from_bytes(" : ") << converter.from_bytes(std::to_string(result)) << converter.from_bytes('\n');
                countMatch++;
            }

        }
        state.counters["MATCHES"] = countMatch;
        state.ResumeTiming();
    }
}

BENCHMARK_DEFINE_F(TearDownFixture, BenchmarkWithoutSimTable)(benchmark::State& state){
    for (auto _ : state) {
        state.PauseTiming();
        resultFile << requestWord << converter.from_bytes(" : 0") << converter.from_bytes('\n');
        std::wstring comp;
        size_t countMatch = 0;
        while(getline(text, comp)) {
            state.ResumeTiming();
            auto result = editDistance(requestWord, comp);
            state.PauseTiming();

            if (result < state.range(1)) {
                resultFile << comp << converter.from_bytes(" : ") << converter.from_bytes(std::to_string(result)) << converter.from_bytes('\n');
                countMatch++;
            }
        }
        state.counters["MATCHES"] = countMatch;
        state.ResumeTiming();
    }
}

static void BM_TABLE(benchmark::State& state) {
    for (auto _ : state) {
        new SimTable("..\\..\\simtable.json");
    }
}

BENCHMARK(BM_TABLE)->Unit(benchmark::kMillisecond);;

BENCHMARK_REGISTER_F(SetUpFixture, BenchmarkWithSimTable)
->ArgsProduct({
                      benchmark::CreateDenseRange(0, 100, 20),
                      benchmark::CreateDenseRange(1, 3, 1)
              })
->Unit(benchmark::kMillisecond)
->Iterations(1);

BENCHMARK_REGISTER_F(TearDownFixture, BenchmarkWithoutSimTable)
->ArgsProduct({
    benchmark::CreateDenseRange(0, 100, 20),
    benchmark::CreateDenseRange(1, 3, 1)
})
->Unit(benchmark::kMillisecond)
->Iterations(1);

BENCHMARK_MAIN();