#include "Basic/AppRoot.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>

using namespace mentor;
using namespace mentor::tui;
using namespace ftxui;

int main()
{
    AppContext ac;
    ac.pm.navigate(Page::welcome);

    auto root = Make<AppRoot>(ac);
    auto app  = App::Fullscreen();
    app.Loop(root);
}
