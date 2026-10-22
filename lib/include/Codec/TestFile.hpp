#pragma once

#include "Kernel/File/TestFile.hpp"

namespace mentor::codec
{

std::optional<kernel::TestFile> decodeTestFile(const nlohmann::json &j);

template <typename File>
concept IsTestFile = std::same_as<File, kernel::TestFile>;

template <typename File>
std::optional<File> decode(const nlohmann::json &j)
    requires IsTestFile<File>
{
    return decodeTestFile(j);
}

} // namespace mentor::codec
