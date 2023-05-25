
#include "tools.h"

#include <vector>

namespace
{
    const float insertCost = 1.f;
    const float deleteCost = 1.f;
    const float replaceCost = 1.f;
    const float transpositionCost = 1.f;

    template<typename T>
    struct Matrix
    {
        Matrix(size_t aLen, size_t bLen) : row(aLen), col(bLen), innerVec(std::vector<T>(row * col, 0.f))
        {}

        T& at(size_t i, size_t j)
        {
            return innerVec[i * col + j];
        }

    private:
        const size_t row;
        const size_t col;
        std::vector<T> innerVec;
    };

    float compareWchar(wchar_t c1, wchar_t c2, const SimTable& table)
    {
        if (c1 == c2) return 0.f;

        const auto similatiry = table.getReplaceCost(c1, c2);
        if (similatiry.has_value())
        {
            return std::min(similatiry.value(), replaceCost);
        }

        return replaceCost;
    }
}

float editDistance(std::wstring_view a, std::wstring_view b)
{
    const size_t aLen = a.length() + 1;
    const size_t bLen = b.length() + 1;

    Matrix<float> matr(aLen, bLen);

    for (size_t i = 0; i < aLen; ++i)
        matr.at(i, 0) = i;

    for (size_t j = 0; j < bLen; ++j)
        matr.at(0, j) = j;

    // building matrix
    for (size_t i = 1; i < aLen; ++i)
    {
        for (size_t j = 1; j < bLen; ++j)
        {
            float compareCharCost = a[i - 1] == b[j - 1] ? 0.f : replaceCost;

            // replace, insert, delete
            matr.at(i, j) = std::min({matr.at(i - 1, j - 1) + compareCharCost,
                                      matr.at(i, j - 1) + insertCost,
                                      matr.at(i - 1, j) + deleteCost});

            // transposition
            if (i > 1 && j > 1 && (a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]))
                matr.at(i, j) = std::min(matr.at(i, j), matr.at(i - 2, j - 2) + transpositionCost);
        }
    }

    return  matr.at(aLen - 1, bLen - 1);
}

float editDistance(std::wstring_view a, std::wstring_view b, const SimTable& table)
{
    const size_t aLen = a.length() + 1;
    const size_t bLen = b.length() + 1;

    Matrix<float> matr(aLen, bLen);

    for (size_t i = 0; i < aLen; ++i)
        matr.at(i, 0) = i;

    for (size_t j = 0; j < bLen; ++j)
        matr.at(0, j) = j;

    // building matrix
    for (size_t i = 1; i < aLen; ++i)
    {
        for (size_t j = 1; j < bLen; ++j)
        {
            float compareCharCost = compareWchar(a[i - 1], b[j - 1], table);

            // replace, insert, delete
            matr.at(i, j) = std::min({matr.at(i - 1, j - 1) + compareCharCost,
                                      matr.at(i, j - 1) + insertCost,
                                      matr.at(i - 1, j) + deleteCost});

            // transposition
            if (i > 1 && j > 1 && (a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]))
                matr.at(i, j) = std::min(matr.at(i, j), matr.at(i - 2, j - 2) + transpositionCost);
        }
    }

    return  matr.at(aLen - 1, bLen - 1);
}
