#pragma once

#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Component/Choice.hpp"
#include "Page/SelectDirPage.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

inline ftxui::Component getSelectCurDirPage(AppContext &ac) noexcept
{
    auto yes = [&]()
    {
        auto json_filter = [](const std::filesystem::path &p)
        {
            return p.extension() == ".json";
        };
        if (auto files = ac.fs.getCurrentDirFiles(json_filter); files)
        {
            ac.files = std::move(*files);
            ac.list.clear();
            for (const auto &f : ac.files)
                ac.list.emplace_back(f.stem().string());
            ac.list_selected = 0;
            ac.pm.navigate(Page::select_file);
        }
        else
        {
            ac.error_info     = "未在当前目录下扫描到测试文件";
            ac.error_on_click = [&ac]
            {
                ac.pm.back();
            };
            ac.pm.navigate(Page::error);
        }
    };
    auto no = [&]()
    {
        auto it = std::filesystem::directory_iterator(ac.cur_path);
        buildListFromIterator(ac, it);
        ac.pm.navigate(Page::select_dir);
    };

    return getChoiceComponent("扫描当前目录下的测试文件？", yes, no);
}

} // namespace mentor::tui
