#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Event/LoadTestFile.hpp"
#include "Kernel/Kernel.hpp"
#include "Service/FileService.hpp"
#include <doctest/doctest.h>
#include <memory>

using namespace mentor;
using namespace mentor::kernel;
using namespace mentor::service;
using namespace mentor::event;

TEST_CASE("testing kernel load test file")
{
    FileService fs;
    Kernel k(fs);

    SUBCASE("invalid test file")
    {
        auto fid      = fs.readJsonFile("test_file_invalid.json");
        auto payload  = std::make_unique<LoadTestFile>(fid);
        auto response = k.handle(toEvent(EventType::load_test_file, payload));
        CHECK(response.type == ResponseType::load_file_failed);
        CHECK(k.getState() == State::init);
    }

    auto fid      = fs.readJsonFile("test_file_valid.json");
    auto payload  = std::make_unique<LoadTestFile>(fid);
    auto response = k.handle(toEvent(EventType::load_test_file, payload));
    CHECK(response.type == ResponseType::load_file_succeed);
    CHECK(k.getState() == State::testing);
    auto testinfo = static_cast<TestInfo *>(response.payload);
    CHECK(testinfo->name == "test");
    CHECK(testinfo->author == "TPPPP");
    CHECK(testinfo->description == "this is a test");
    CHECK(testinfo->id == 99);
    CHECK(testinfo->mode == TestMode::practice);
    CHECK(testinfo->score == 50);

    SUBCASE("invalid inner state")
    {
        auto response = k.handle(toEvent(EventType::load_test_file, payload));
        CHECK(response.type == ResponseType::error);
    }
}
