#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

inline ftxui::Component getChoiceComponent(std::string_view title,
                                           std::function<void()> on_yes,
                                           std::function<void()> on_no)
{
    using namespace ftxui;
    auto buttons = Container::Vertical({
        Button("是", [on_yes]
               {
                   on_yes();
               }),
        Button("否", [on_no]
               {
                   on_no();
               }),
    });

    return Renderer(buttons, [buttons, title]
                    {
                        return vbox({
                                   text(title) | bold,
                                   separator(),
                                   buttons->Render(),
                                   text("（↑↓ 选择，Enter 确认）") | dim,
                               }) |
                               border | center;
                    });
}

} // namespace mentor::tui
