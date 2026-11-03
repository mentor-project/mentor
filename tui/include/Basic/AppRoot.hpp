#pragma once

#include "Basic/AppContext.hpp"
#include "Basic/PageEnum.hpp"
#include "Page/EnsureSelectDirPage.hpp"
#include "Page/ErrorPage.hpp"
#include "Page/Event/EventHandler.hpp"
#include "Page/SelectCurDirPage.hpp"
#include "Page/SelectDirPage.hpp"
#include "Page/SelectFilePage.hpp"
#include "Page/WelcomePage.hpp"
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <unordered_map>

namespace mentor::tui
{

class AppRoot : public ftxui::ComponentBase
{
public:
    AppRoot(AppContext &ac) : m_ac(ac), m_eh(ac)
    {
        m_pages.emplace(Page::welcome, getWelcomePage(ac));
        m_pages.emplace(Page::select_cur_dir, getSelectCurDirPage(ac));
        m_pages.emplace(Page::select_file, getSelectFilePage(ac));
        m_pages.emplace(Page::select_dir, getSelectDirPage(ac));
        m_pages.emplace(Page::ensure_select_dir, getEnsureSelectDirPage(ac));
        m_pages.emplace(Page::test_preview, ftxui::Renderer([]
                                                            {
                                                                return ftxui::text("test_preview");
                                                            }));
        m_pages.emplace(Page::error, getErrorPage(ac));

        ac.cur_path = "C:/";
    }

    ftxui::Element OnRender() override
    {
        return m_pages[m_ac.pm.getCurrentPage()]->Render();
    }

    bool OnEvent(ftxui::Event e) override
    {
        if (m_eh.handle(e))
            return true;
        return m_pages[m_ac.pm.getCurrentPage()]->OnEvent(e);
    }

private:
    std::unordered_map<Page, ftxui::Component> m_pages;
    AppContext &m_ac;
    EventHandler m_eh;
};

} // namespace mentor::tui
