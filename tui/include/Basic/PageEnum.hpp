#pragma once

#include <cstdint>

namespace mentor::tui
{

enum class Page : uint8_t
{
    // Standard
    welcome,
    select_cur_dir,
    select_dir,
    ensure_select_dir,
    select_file,
    test_preview,
    choice_question,
    essay_question,

    // Error handling
    error,

    // No page
    empty
};

} // namespace mentor::tui
