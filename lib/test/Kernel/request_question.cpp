#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Event/LoadTestFile.hpp"
#include "Event/RequestQuestion.hpp"
#include "Kernel/Kernel.hpp"
#include "Service/FileService.hpp"
#include <doctest/doctest.h>
#include <memory>

using namespace mentor;
using namespace mentor::kernel;
using namespace mentor::service;
using namespace mentor::event;

TEST_CASE("testing kernel request question")
{
    FileService fs;
    Kernel k(fs);

    {
        auto fid      = fs.readJsonFile("test_file_valid.json");
        auto payload  = std::make_unique<LoadTestFile>(fid);
        auto response = k.handle(toEvent(EventType::load_test_file, payload));
    }

    SUBCASE("request choice question")
    {
        auto payload  = std::make_unique<RequestQuestion>(0);
        auto response = k.handle(toEvent(EventType::request_question, payload));
        CHECK(response.type == ResponseType::post_choice_question);
        auto handle = static_cast<QuestionHandle *>(response.payload);

        {
            auto q = k.tryGetQuestion<EssayQuestion>(*handle);
            CHECK(!q);
        }

        auto q = k.tryGetQuestion<ChoiceQuestion>(*handle);
        CHECK(q->id == 1);
        CHECK(q->stem == "s");
        CHECK(q->score == 10);
        CHECK(q->options[0] == "op1");
        CHECK(q->options[1] == "op2");
        CHECK(q->hint == "h");
        CHECK(q->answer == 0);
        CHECK(q->solution == "sol");
    }

    SUBCASE("request essay question")
    {
        auto payload  = std::make_unique<RequestQuestion>(1);
        auto response = k.handle(toEvent(EventType::request_question, payload));
        CHECK(response.type == ResponseType::post_essay_question);
        auto handle = static_cast<QuestionHandle *>(response.payload);

        {
            auto q = k.tryGetQuestion<ChoiceQuestion>(*handle);
            CHECK(!q);
        }

        auto q = k.tryGetQuestion<EssayQuestion>(*handle);
        CHECK(q->id == 1);
        CHECK(q->stem == "s");
        CHECK(q->score == 10);
        CHECK(q->hint == "h");
        CHECK(q->answers[0] == "12");
        CHECK(q->answers[1] == "3");
        CHECK(q->solution == "sol");
    }
}
