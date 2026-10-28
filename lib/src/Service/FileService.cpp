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

std::optional<std::vector<std::filesystem::path>> FileService::getCurrentDirFiles(bool is_recursive)
{
    return getDirFiles(std::filesystem::current_path(), nullptr, is_recursive);
}

json_file_id FileService::readJsonFile(const std::filesystem::path &p)
{
    std::ifstream i(p);
    if (!i.is_open())
        return -1;

    std::stringstream ss;
    ss << i.rdbuf();
    try
    {
        auto it = m_json_file_cache.try_emplace(m_json_id, nlohmann::json::parse(ss)).first;
        ++m_json_id;
        return it->first;
    }
    catch (...)
    {
        return -1;
    }
}

nlohmann::json *FileService::getJsonFile(json_file_id fid)
{
    auto it = m_json_file_cache.find(fid);

    if (it == m_json_file_cache.end())
        return nullptr;

    return &it->second;
}

} // namespace mentor::service
