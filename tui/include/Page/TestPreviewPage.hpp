#pragma once

#include "Basic/AppContext.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

inline std::string_view test_mode_to_string(TestMode mode)
{
    switch (mode)
    {
    case TestMode::unspecified:
        return "未指定";
    case TestMode::test:
        return "测试";
    case TestMode::practice:
        return "练习";
    }
    return "";
}

inline ftxui::Component getTestPreviewPage(AppContext &ac)
{
    using namespace ftxui;

    return Renderer([&ac]
                    {
                        if (!ac.tinfo)
                            return text("没有测试信息");

                        const auto &info = *ac.tinfo;

                        Elements rows;

                        auto add_row = [&rows](const std::string &label, const std::string &value)
                        {
                            if (value.empty())
                                return;
                            rows.push_back(hbox({
                                text(label) | bold | size(WIDTH, EQUAL, 8),
                                text(value) | flex,
                            }));
                        };

                        add_row("名称", info.name);
                        add_row("作者", info.author);

                        rows.push_back(hbox({
                            text("模式") | bold | size(WIDTH, EQUAL, 8),
                            text(test_mode_to_string(info.mode)) | flex,
                        }));

                        add_row("ID", std::to_string(info.id));
                        add_row("分数", std::to_string(info.score));

                        if (!info.description.empty())
                        {
                            rows.push_back(separator());
                            rows.push_back(text("描述") | bold);
                            rows.push_back(paragraph(info.description) | flex);
                        }

                        return vbox({
                                   text("测试信息预览") | bold | center,
                                   separator(),
                                   vbox(std::move(rows)) | flex,
                                   separator(),
                                   text("按回车键继续") | dim
                               }) |
                               border | size(WIDTH, LESS_THAN, 60) | center;
                    });
}

inline bool handleTestPreviewPageEvent(AppContext &ac, const ftxui::Event &e)
{
    if (e == ftxui::Event::Return)
    {
        ac.pm.navigate(Page::choice_question);
        return true;
    }
    return false;
}

} // namespace mentor::tui
