#pragma once

#include "Basic/PageManager.hpp"
#include "Service/FileService.hpp"
#include "Kernel/Kernel.hpp"
#include <functional>
#include <string>
#include <vector>

namespace mentor::tui
{

struct AppContext
{
    kernel::Kernel kernel;
    service::FileService fs;
    PageManager pm;
    std::filesystem::path cur_path;
    std::string error_info;
    std::vector<std::filesystem::path> files;
    std::vector<std::string> list;
    std::function<void()> error_on_click;
    TestInfo *tinfo;
    kernel::QuestionHandle cur_question;
    kernel::question_index cur_question_idx{};
    int list_selected{};

    AppContext() : kernel(fs) {}
};

} // namespace mentor::tui
