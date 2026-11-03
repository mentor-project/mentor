#pragma once

#include "Basic/AppContext.hpp"
#include "Page/ErrorPage.hpp"
#include "Page/SelectDirPage.hpp"
#include "Page/TestPreviewPage.hpp"
#include "Page/WelcomePage.hpp"
#include <ftxui/component/event.hpp>

namespace mentor::tui
{

class EventHandler
{
public:
    EventHandler(AppContext &ac) : m_ac(ac) {}

    bool handle(const ftxui::Event &e) noexcept
    {
        auto &pm      = m_ac.pm;
        auto cur_page = pm.getCurrentPage();
        switch (cur_page)
        {
        case Page::welcome:
            return handleWelcomePageEvent(m_ac, e);
        case Page::select_dir:
            return handleSelectDirPageEvent(m_ac, e);
        case Page::test_preview:
            return handleTestPreviewPageEvent(m_ac, e);
        case Page::error:
            return handleErrorPageEvent(m_ac, e);
        default:
            return false;
        }
    }

private:
    AppContext &m_ac;
};

} // namespace mentor::tui
