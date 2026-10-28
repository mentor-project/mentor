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
    k.setState(State::WaitForChooseFile);

    SUBCASE("invalid test file")
    {
        auto fid      = fs.readJsonFile("test_file_invalid.json");
        auto payload  = std::make_unique<LoadTestFile>(fid);
        auto response = k.handle(toEvent(EventType::load_test_file, payload));
        CHECK(response.type == ResponseType::load_file_failed);
    }

    auto fid      = fs.readJsonFile("test_file_valid.json");
    auto payload  = std::make_unique<LoadTestFile>(fid);
    auto response = k.handle(toEvent(EventType::load_test_file, payload));
    CHECK(response.type == ResponseType::load_file_succeed);
}
