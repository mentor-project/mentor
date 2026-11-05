#pragma once

#include "Basic/AppContext.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

namespace mentor::tui
{

inline ftxui::Component getErrorPage(AppContext &ac) noexcept
{
    using namespace ftxui;

    return Renderer([&ac]
                    {
                        return vbox(text(ac.error_info) | bold, separator(), text("按回车键继续")) |
                               border | center;
                    });
}

inline bool handleErrorPageEvent(AppContext &ac, const ftxui::Event &e) noexcept
{
    if (e == ftxui::Event::Return)
    {
        if (ac.error_on_click)
            ac.error_on_click();
        return true;
    }
    return false;
}

} // namespace mentor::tui
