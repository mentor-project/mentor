#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

int main()
{
    using namespace ftxui;

    std::vector<std::string> entries = {
        "Entry 1",
        "Entry 2",
        "Entry 3",
    };
    int selected = 0;

    // Create a menu component
    auto menu = Menu(&entries, &selected);

    // You can decorate components using the pipe operator.
    auto component = menu | border;

    // Start the main loop
    auto app = App::TerminalOutput();
    app.Loop(component);
}
