#pragma once

#include "Base/Protocol/TestFile.hpp"

namespace mentor::codec
{

std::optional<TestFile> decodeTestFile(const nlohmann::json &j);

template <typename File>
concept IsTestFile = std::same_as<File, TestFile>;

template <typename File>
std::optional<File> decode(const nlohmann::json &j)
    requires IsTestFile<File>
{
    return decodeTestFile(j);
}

} // namespace mentor::codec
