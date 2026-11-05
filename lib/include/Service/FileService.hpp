#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

namespace mentor::service
{

using json_file_id = int32_t;

class FileService
{
public:
    std::optional<std::vector<std::filesystem::path>> getDirFiles(const std::filesystem::path &,
                                                                  std::function<bool(const std::filesystem::path &p)> = nullptr,
                                                                  bool                                                = true) noexcept;
    std::optional<std::vector<std::filesystem::path>> getCurrentDirFiles(std::function<bool(const std::filesystem::path &)> = nullptr, bool = true) noexcept;
    json_file_id readJsonFile(const std::filesystem::path &) noexcept;
    const nlohmann::json *getJsonFile(json_file_id) const noexcept;

private:
    std::unordered_map<std::filesystem::path, std::vector<std::filesystem::path>> m_path_cache;
    std::unordered_map<json_file_id, nlohmann::json> m_json_file_cache;
    json_file_id m_json_id{};
};

} // namespace mentor::service
