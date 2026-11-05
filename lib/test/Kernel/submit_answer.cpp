#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Event/LoadTestFile.hpp"
#include "Event/RequestQuestion.hpp"
#include "Event/SubmitAnswer.hpp"
#include "Kernel/Kernel.hpp"
#include "Response/CheckResult.hpp"
#include "Service/FileService.hpp"
#include <doctest/doctest.h>
#include <memory>

using namespace mentor;
using namespace mentor::kernel;
using namespace mentor::service;
using namespace mentor::event;

TEST_CASE("testing kernel submit answer")
{
    FileService fs;
    Kernel k(fs);

    {
        auto fid     = fs.readJsonFile("test_file_valid.json");
        auto payload = std::make_unique<LoadTestFile>(fid);
        k.handle(toEvent(EventType::load_test_file, payload));
    }

    for (int i = 0; i < 3; ++i)
    {
        auto payload = std::make_unique<RequestQuestion>(i);
        k.handle(toEvent(EventType::request_question, payload));
    }

    SUBCASE("check single choice question")
    {
        SUBCASE("check wrong answer")
        {
            auto payload  = std::make_unique<event::SubmitAnswer>(0u, 2u);
            auto response = k.handle(toEvent(EventType::submit_answer, payload));
            auto result   = response.payload.get<response::CheckResult *>();
            CHECK(response.type == ResponseType::wrong);
            CHECK(result->answer == "A");
            CHECK(result->correctness == 0.0f);
        }
        auto payload  = std::make_unique<event::SubmitAnswer>(0u, 1u);
        auto response = k.handle(toEvent(EventType::submit_answer, payload));
        auto result   = response.payload.get<response::CheckResult *>();
        CHECK(response.type == ResponseType::correct);
        CHECK(!result);
    }

    SUBCASE("check multiple choice question")
    {
        SUBCASE("check partial wrong answer")
        {
            auto payload  = std::make_unique<event::SubmitAnswer>(2u, 1u);
            auto response = k.handle(toEvent(EventType::submit_answer, payload));
            auto result   = response.payload.get<response::CheckResult *>();
            CHECK(response.type == ResponseType::wrong);
            CHECK(result->answer == "A、B");
            CHECK(result->correctness == 0.5f);
        }
        SUBCASE("check wrong answer")
        {
            auto payload  = std::make_unique<event::SubmitAnswer>(2u, 5u);
            auto response = k.handle(toEvent(EventType::submit_answer, payload));
            auto result   = response.payload.get<response::CheckResult *>();
            CHECK(response.type == ResponseType::wrong);
            CHECK(result->answer == "A、B");
            CHECK(result->correctness == 0.0f);
        }
        auto payload  = std::make_unique<event::SubmitAnswer>(2u, 3u);
        auto response = k.handle(toEvent(EventType::submit_answer, payload));
        auto result   = response.payload.get<response::CheckResult *>();
        CHECK(response.type == ResponseType::correct);
        CHECK(!result);
    }
}
