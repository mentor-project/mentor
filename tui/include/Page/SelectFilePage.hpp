#pragma once

#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Component/List.hpp"
#include "Event/LoadTestFile.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace mentor::tui
{

inline ftxui::Component getSelectFilePage(AppContext &ac)
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

        Event e;
        e.type        = EventType::load_test_file;
        auto payload  = std::make_unique<event::LoadTestFile>(fid);
        e.payload     = payload.get();
        auto response = ac.kernel.handle(e);
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
        ac.tinfo = static_cast<TestInfo *>(response.payload);
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
                            text("Enter: 确认") | dim,
                        });
                    });
}

} // namespace mentor::tui
