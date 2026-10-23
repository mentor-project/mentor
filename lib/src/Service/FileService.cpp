#include "Service/FileService.hpp"
#include <fstream>
#include <sstream>

namespace mentor::service
{

std::optional<std::vector<std::filesystem::path>> FileService::getDirFiles(const std::filesystem::path &p,
                                                                           std::function<bool(const std::filesystem::path &p)> filter,
                                                                           bool is_recursive)
{
    if (!std::filesystem::is_directory(p))
        return std::nullopt;

    std::vector<std::filesystem::path> files;

    auto collect = [&files, filter]<typename Iter>(Iter it)
    {
        for (const auto &entry : it)
        {
            if (entry.is_regular_file())
            {
                if (!filter || filter(entry))
                    files.push_back(entry);
            }
        }
    };

    if (is_recursive)
        collect(std::filesystem::recursive_directory_iterator(p));
    else
        collect(std::filesystem::directory_iterator(p));

    return files;
}

std::optional<std::vector<std::filesystem::path>> FileService::getCurrentDirFiles()
{
    return getDirFiles(std::filesystem::current_path());
}

std::optional<nlohmann::json> FileService::readJsonFile(const std::filesystem::path &p)
{
    std::ifstream i(p);
    if (!i.is_open())
        return std::nullopt;

    std::stringstream ss;
    ss << i.rdbuf();
    try
    {
        nlohmann::json j = nlohmann::json::parse(ss);
        return j;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

} // namespace mentor::service
