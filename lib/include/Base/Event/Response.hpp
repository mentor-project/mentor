#pragma once

namespace mentor
{

enum class ResponseType
{
    error,
    load_file_succeed,
    load_file_failed,
    post_question_succeed,
    post_question_failed
};

struct Response
{
    ResponseType type;
    void *payload{};

    // we do not put payload when meeting error for now
    static Response error() noexcept
    {
        return {ResponseType::error};
    }
};

} // namespace mentor
