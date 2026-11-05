#pragma once

#include <bit>
#include <cstdint>
#include <string>
#include <vector>

namespace mentor
{

enum class TestMode : uint32_t
{
    unspecified,
    practice,
    test
};

struct TestInfo
{
    std::string name;
    std::string author;
    std::string description;
    TestMode mode;
    uint32_t id;
    uint32_t score;
    uint32_t question_count;
};

enum class QuestionType : uint32_t
{
    unknown,
    choice,
    essay
};

struct ChoiceQuestion
{
    std::string stem;
    std::string hint;
    std::string solution;
    std::vector<std::string> options;
    uint32_t answer;
    uint32_t id;
    uint32_t score;

    bool isMultiple() const noexcept{
        return std::popcount(answer) > 1;
    }
};

struct EssayQuestion
{
    std::string stem;
    std::string hint;
    std::string solution;
    std::vector<std::string> answers;
    uint32_t id;
    uint32_t score;
};

} // namespace mentor
