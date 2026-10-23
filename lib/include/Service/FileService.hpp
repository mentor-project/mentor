#pragma once

#include <filesystem>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

namespace mentor::service
{

class FileService
{
public:
    static std::optional<std::vector<std::filesystem::path>> getDirFiles(const std::filesystem::path &p,
                                                                         std::function<bool(const std::filesystem::path &p)> filter = nullptr,
                                                                         bool is_recursive                                          = true);

    static std::optional<std::vector<std::filesystem::path>> getCurrentDirFiles();

    static std::optional<nlohmann::json> readJsonFile(const std::filesystem::path &p);
};

} // namespace mentor::service
