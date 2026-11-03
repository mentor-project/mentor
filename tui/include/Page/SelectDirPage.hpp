#pragma once

#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Component/List.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

namespace mentor::tui
{

inline ftxui::Component getSelectDirPage(AppContext &ac)
{
    auto buildListFromCurPath = [&ac]()
    {
        ac.files.clear();
        ac.list.clear();
        ac.list_selected = 0;
        auto it          = std::filesystem::directory_iterator(ac.cur_path);
        for (const auto &entry : it)
        {
            auto p = static_cast<std::filesystem::path>(entry);
            ac.files.emplace_back(p);
            if (std::filesystem::is_directory(p))
                ac.list.emplace_back(p.filename().string() + "/");
            else
                ac.list.emplace_back(p.filename().string());
        }
    };
    auto entry = [&ac, buildListFromCurPath](const std::string &s)
    {
        ac.cur_path = ac.files[ac.list_selected];
        if (!std::filesystem::is_directory(ac.cur_path))
        {
            ac.error_info     = "请不要进入文件";
            ac.error_on_click = [&ac, buildListFromCurPath]()
            {
                ac.pm.back();
                ac.cur_path = ac.cur_path.parent_path();
                buildListFromCurPath();
            };
            ac.pm.navigate(Page::error);
            return;
        }
        buildListFromCurPath();
    };
    auto list = getListComponent(ac.list, ac.list_selected, entry);

    using namespace ftxui;

    return Renderer(list, [list]
                    {
                        return vbox({
                            text("目录视图") | bold | center,
                            separator(),
                            list->Render() | flex,
                            separator(),
                            text("Backspace: 返回上一级    E: 选择当前目录") | dim,
                        });
                    });
}

inline bool handleSelectDirPageEvent(AppContext &ac, const ftxui::Event &e)
{
    if (e == ftxui::Event::Backspace)
    {
        auto parent = ac.cur_path.parent_path();
        if (parent.empty())
            return true;

        ac.files.clear();
        ac.list.clear();
        ac.list_selected = 0;
        ac.cur_path = parent;

        auto it = std::filesystem::directory_iterator(parent);
        for (const auto &entry : it)
        {
            auto p = static_cast<std::filesystem::path>(entry);
            ac.files.emplace_back(p);
            if (std::filesystem::is_directory(p))
                ac.list.emplace_back(p.filename().string() + "/");
            else
                ac.list.emplace_back(p.filename().string());
        }
        return true;
    }

    if (e == ftxui::Event::Character('e') || e == ftxui::Event::Character('E'))
    {
        ac.pm.navigate(Page::ensure_select_dir);
        return true;
    }

    return false;
}

} // namespace mentor::tui
