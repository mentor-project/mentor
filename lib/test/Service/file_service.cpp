#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Service/FileService.hpp"
#include <doctest/doctest.h>

using namespace mentor::service;
using json = nlohmann::json;

TEST_CASE("testing getDirFiles")
{
    FileService fs;

    SUBCASE("invalid path")
    {
        auto empty_dir = fs.getDirFiles("\0");
        CHECK(!empty_dir.has_value());
    }

    SUBCASE("not recursive dir")
    {
        auto dummy = fs.getDirFiles("dummy", nullptr, false);
        REQUIRE(dummy.has_value());
        CHECK(dummy->size() == 1);
        CHECK((*dummy)[0].filename() == "1.txt");
    }

    SUBCASE("recursive dir")
    {
        auto dummy = fs.getDirFiles("dummy");
        REQUIRE(dummy.has_value());
        CHECK(dummy->size() == 2);
    }

    SUBCASE("filiter")
    {
        auto dummy = fs.getDirFiles("dummy", [](const std::filesystem::path &p)
                                    {
                                        return p.extension() == ".md";
                                    });
        REQUIRE(dummy.has_value());
        CHECK(dummy->size() == 1);
        CHECK((*dummy)[0].filename() == "2.md");
    }
}

TEST_CASE("testing getCurrentDirFiles")
{
    FileService fs;
    
    SUBCASE("not recursive")
    {
        auto dummy = fs.getCurrentDirFiles(false);
        REQUIRE(dummy.has_value());
        CHECK(dummy->size() > 2);
        CHECK(std::find_if(dummy->begin(), dummy->end(), [](const std::filesystem::path &p)
                           {
                               return p.filename() == "1.txt";
                           }) == dummy->end());
    }

    SUBCASE("recursive")
    {
        auto dummy = fs.getCurrentDirFiles();
        REQUIRE(dummy.has_value());
        CHECK(dummy->size() > 2);
        CHECK(std::find_if(dummy->begin(), dummy->end(), [](const std::filesystem::path &p)
                           {
                               return p.filename() == "1.txt";
                           }) != dummy->end());
    }
}

TEST_CASE("testing JsonFile")
{
    FileService fs;

    SUBCASE("read inexist file")
    {
        auto fid = fs.readJsonFile("inexist.json");
        CHECK(fid == -1);
    }

    SUBCASE("read invalid file")
    {
        auto fid = fs.readJsonFile("fstest_invalid.json");
        CHECK(fid == -1);
    }

    auto fid1 = fs.readJsonFile("fstest_valid1.json");
    CHECK(fid1 != -1);
    auto fid2 = fs.readJsonFile("fstest_valid2.json");
    CHECK(fid2 != -1);

    SUBCASE("get inexist jid")
    {
        CHECK(!fs.getJsonFile(2));
    }

    CHECK(fs.getJsonFile(0));
    CHECK(fs.getJsonFile(1));
}
