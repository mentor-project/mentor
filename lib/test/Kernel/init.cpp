#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Kernel/Kernel.hpp"
#include "Service/FileService.hpp"
#include <doctest/doctest.h>

using namespace mentor::kernel;
using namespace mentor::service;

TEST_CASE("testing kernel init")
{
    FileService fs;
    Kernel k(fs);
    CHECK(k.getState() == State::Init);
}
