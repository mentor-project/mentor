#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

inline ftxui::Component getListComponent(
    std::vector<std::string> &entries,
    int &selected,
    std::function<void(const std::string &)> on_entry)
{
    using namespace ftxui;

    MenuOption option;
    option.on_enter = [&entries, &selected, on_entry]
    {
        if (selected >= 0 && selected < entries.size())
            on_entry(entries[selected]);
    };

    auto menu = Menu(&entries, &selected, option);

    return Renderer(menu, [menu]
                    {
                        return menu->Render() | frame | vscroll_indicator;
                    });
}

} // namespace mentor::tui
