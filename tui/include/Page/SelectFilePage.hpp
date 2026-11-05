#pragma once

#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Component/List.hpp"
#include "Event/LoadTestFile.hpp"
#include "Page/SelectDirPage.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace mentor::tui
{

inline ftxui::Component getSelectFilePage(AppContext &ac) noexcept
{
    auto entry = [&ac](const std::string &s)
    {
        auto fid = ac.fs.readJsonFile(ac.files[ac.list_selected]);
        if (fid == -1)
        {
            ac.error_info     = "反序列化json文件失败";
            ac.error_on_click = [&ac]
            {
                ac.pm.back();
            };
            ac.pm.navigate(Page::error);
            return;
        }

        auto payload  = std::make_unique<event::LoadTestFile>(fid);
        auto response = ac.kernel.handle(toEvent(EventType::load_test_file, payload));
        if (response.type == ResponseType::load_file_failed)
        {
            ac.error_info     = "该json文件不是测试文件";
            ac.error_on_click = [&ac]
            {
                ac.pm.back();
            };
            ac.pm.navigate(Page::error);
            return;
        }
        ac.tinfo = response.payload.get<TestInfo *>();
        ac.pm.navigate(Page::test_preview);
    };

    auto list = getListComponent(ac.list, ac.list_selected, entry);

    using namespace ftxui;

    return Renderer(list, [list]
                    {
                        return vbox({
                            text("json 文件列表") | bold | center,
                            separator(),
                            list->Render() | flex,
                            separator(),
                            text("Backspace: 返回上一级    Enter: 确认") | dim,
                        });
                    });
}

inline bool handleSelectFilePageEvent(AppContext &ac, const ftxui::Event &e) noexcept
{
    if (e == ftxui::Event::Backspace)
    {
        buildListFromIterator(ac, tryGetIter(ac.cur_path));
        ac.pm.backTo(Page::select_cur_dir | Page::select_dir);
        return true;
    }
    return false;
}

} // namespace mentor::tui
