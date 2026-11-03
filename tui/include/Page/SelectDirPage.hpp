#pragma once

#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Component/List.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

inline void buildListFromCurPath(AppContext &ac)
{
    ac.files.clear();
    ac.list.clear();
    ac.list_selected = 0;
    std::error_code ec;
    auto it = std::filesystem::directory_iterator(ac.cur_path, ec);
    if (ec)
    {
        ac.error_info     = "权限不足，无法访问该文件夹";
        ac.error_on_click = [&ac]()
        {
            ac.pm.back();
            ac.cur_path = ac.cur_path.parent_path();
            buildListFromCurPath(ac);
        };
        ac.pm.navigate(Page::error);
        return;
    }
    for (const auto &entry : it)
    {
        auto p = static_cast<std::filesystem::path>(entry);
        ac.files.emplace_back(p);
        if (std::filesystem::is_directory(p))
            ac.list.emplace_back(p.filename().string() + "/");
        else
            ac.list.emplace_back(p.filename().string());
    }
}

inline ftxui::Component getSelectDirPage(AppContext &ac)
{
    auto entry = [&ac](const std::string &s)
    {
        ac.cur_path = ac.files[ac.list_selected];
        if (!std::filesystem::is_directory(ac.cur_path))
        {
            ac.error_info     = "请不要进入文件";
            ac.error_on_click = [&ac]()
            {
                ac.pm.back();
                ac.cur_path = ac.cur_path.parent_path();
                buildListFromCurPath(ac);
            };
            ac.pm.navigate(Page::error);
            return;
        }
        buildListFromCurPath(ac);
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

        ac.cur_path = parent;
        buildListFromCurPath(ac);
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
