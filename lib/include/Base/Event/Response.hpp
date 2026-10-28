#pragma once

namespace mentor
{

enum class ResponseType
{
    error,
    load_file_succeed,
    load_file_failed,
};

struct Response
{
    ResponseType type;
    void *payload;
};

} // namespace mentor
