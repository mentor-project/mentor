#pragma once

#include <cstdint>

namespace mentor::tui
{

enum class Page : uint32_t
{
    // Standard
    welcome = 1,
    select_cur_dir = 1 << 1,
    select_dir = 1 << 2,
    ensure_select_dir = 1 << 3,
    select_file = 1 << 4,
    test_preview = 1 << 5,
    choice_question = 1 << 6,
    essay_question = 1 << 7,

    // Error handling
    error = 1 << 8,

    // No page
    empty = 1 << 9
};

inline uint32_t operator|(Page a, Page b) noexcept
{
    return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
}

} // namespace mentor::tui
