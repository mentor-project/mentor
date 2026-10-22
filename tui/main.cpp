#include "Page/WelcomePage.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>

int main()
{
    auto app      = ftxui::App::Fullscreen();
    auto renderer = ftxui::Renderer(&mentor::tui::getWelcomePage);
    app.Loop(renderer);
}
