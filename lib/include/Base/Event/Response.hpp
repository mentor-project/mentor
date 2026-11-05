#pragma once

#include "Base/Event/Payload.hpp"

namespace mentor
{

enum class ResponseType
{
    error,
    load_file_succeed,
    load_file_failed,
    post_question_succeed,
    post_question_failed,
    correct,
    wrong
};

struct Response
{
    ResponseType type;
    Payload payload;

    // we do not put payload when meeting error for now
    static Response error() noexcept
    {
        return {ResponseType::error};
    }
};

} // namespace mentor
