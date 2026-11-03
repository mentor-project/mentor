#pragma once

#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Component/Choice.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

inline ftxui::Component getEnsureSelectDirPage(AppContext &ac)
{
    auto yes = [&]()
    {
        auto json_filter = [](const std::filesystem::path &p)
        {
            return p.extension() == ".json";
        };
        if (auto files = ac.fs.getDirFiles(ac.cur_path, json_filter); files)
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
                ac.pm.back();
            };
            ac.pm.navigate(Page::error);
        }
    };
    auto no = [&]()
    {
        ac.pm.back();
    };

    return getChoiceComponent("确定选择当前目录", yes, no);
}

} // namespace mentor::tui
