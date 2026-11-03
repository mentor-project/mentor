#pragma once

#include "Basic/PageEnum.hpp"
#include <stack>

namespace mentor::tui
{

class PageManager
{
public:
    [[nodiscard]] Page getCurrentPage() const noexcept
    {
        if (m_pst.empty())
            return Page::empty;

        return m_pst.top();
    }

    bool back() noexcept
    {
        if (m_pst.empty())
            return false;

        m_pst.pop();
        return true;
    }

    void navigate(Page p) noexcept
    {
        m_pst.push(p);
    }

private:
    std::stack<Page> m_pst;
};

} // namespace mentor::tui
