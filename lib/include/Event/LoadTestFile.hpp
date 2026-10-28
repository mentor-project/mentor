#pragma once

#include <nlohmann/json.hpp>
#include "Service/FileService.hpp"

namespace mentor::event
{

struct LoadTestFile
{
    service::json_file_id fid;
};

} // namespace mentor::event
