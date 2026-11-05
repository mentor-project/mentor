#pragma once

#include "Kernel/Kernel.hpp"

namespace mentor::event
{

struct SubmitAnswer
{
    std::string line;
    kernel::question_index qi;
    uint32_t li;
    uint32_t choice;

    SubmitAnswer() = default;
    SubmitAnswer(kernel::question_index i, uint32_t c) : qi(i), choice(c) {}
    SubmitAnswer(kernel::question_index i, uint32_t li, const std::string &l) : line(l), qi(i), li(li) {}
};

} // namespace mentor::event
