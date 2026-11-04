#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Codec/Model.hpp"
#include <doctest/doctest.h>

using namespace mentor;
using namespace mentor::codec;
using json = nlohmann::json;

TEST_CASE("testing decoding of TestInfo")
{
    auto j    = json::parse(R"(
        {
            "id": 99,
            "name": "test",
            "author": "TPPPP",
            "description": "this is a test",
            "score": 50,
            "mode": 1
        }
    )");
    auto info = *decode<TestInfo>(j);
    CHECK(info.name == "test");
    CHECK(info.author == "TPPPP");
    CHECK(info.description == "this is a test");
    CHECK(info.mode == TestMode::practice);
    CHECK(info.id == 99);
    CHECK(info.score == 50);

    SUBCASE("default json")
    {
        auto j    = json::parse(R"({"id": 1})");
        auto info = *decode<TestInfo>(j);
        CHECK(info.name.empty());
        CHECK(info.author.empty());
        CHECK(info.description.empty());
        CHECK(info.mode == TestMode::unspecified);
        CHECK(info.id == 1);
        CHECK(info.score == 100);
    }

    SUBCASE("empty json")
    {
        auto j    = json::parse(R"({})");
        auto info = decode<TestInfo>(j);
        CHECK(!info);
    }
}

TEST_CASE("testing decoding of ChoiceQuestion")
{
    auto j        = json::parse(R"(
        {
            "type": 1,
            "id": 1,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "options": ["op1", "op2"],
            "answer": 0,
            "score": 10
        }
    )");
    auto question = *decode<ChoiceQuestion>(j);
    CHECK(question.id == 1);
    CHECK(question.stem == "s");
    CHECK(question.solution == "sol");
    CHECK(question.hint == "h");
    CHECK(question.options[0] == "op1");
    CHECK(question.options[1] == "op2");
    CHECK(question.answer == 0);
    CHECK(question.score == 10);

    SUBCASE("no id json")
    {
        auto j        = json::parse(R"(
        {
            "type": 1,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "options": ["op1", "op2"],
            "answer": 0,
            "score": 10
        }
    )");
        auto question = decode<ChoiceQuestion>(j);
        CHECK(!question);
    }

    SUBCASE("no answer json")
    {
        auto j        = json::parse(R"(
        {
            "type": 1,
            "id": 1,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "options": ["op1", "op2"],
            "score": 10
        }
    )");
        auto question = decode<ChoiceQuestion>(j);
        CHECK(!question);
    }

    SUBCASE("no type json")
    {
        auto j        = json::parse(R"(
        {
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "options": ["op1", "op2"],
            "answer": 0,
            "score": 10
        }
    )");
        auto question = decode<ChoiceQuestion>(j);
        CHECK(!question);
    }

    SUBCASE("empty json")
    {
        auto j        = json::parse(R"({})");
        auto question = decode<ChoiceQuestion>(j);
        CHECK(!question);
    }
}

TEST_CASE("testing decoding of EssayQuestion")
{
    auto j        = json::parse(R"(
        {
            "type": 2,
            "id": 1,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "answers": ["12", "3"],
            "score": 10
        }
    )");
    auto question = *decode<EssayQuestion>(j);
    CHECK(question.id == 1);
    CHECK(question.stem == "s");
    CHECK(question.solution == "sol");
    CHECK(question.hint == "h");
    CHECK(question.answers[0] == "12");
    CHECK(question.answers[1] == "3");
    CHECK(question.score == 10);

    SUBCASE("no id json")
    {
        auto j        = json::parse(R"(
        {
            "type": 2,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "answers": ["12", "3"],
            "score": 10
        }
    )");
        auto question = decode<EssayQuestion>(j);
        CHECK(!question);
    }

    SUBCASE("no answers json")
    {
        auto j        = json::parse(R"(
        {
            "type": 2,
            "id": 1,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "score": 10
        }
    )");
        auto question = decode<EssayQuestion>(j);
        CHECK(!question);
    }

    SUBCASE("no type json")
    {
        auto j        = json::parse(R"(
        {
            "id": 1,
            "stem": "s",
            "solution": "sol",
            "hint": "h",
            "answers": ["12", "3"],
            "score": 10
        }
    )");
        auto question = decode<EssayQuestion>(j);
        CHECK(!question);
    }

    SUBCASE("empty json")
    {
        auto j        = json::parse(R"({})");
        auto question = decode<EssayQuestion>(j);
        CHECK(!question);
    }
}
