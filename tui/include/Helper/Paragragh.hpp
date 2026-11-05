#pragma once

#include <ftxui/dom/elements.hpp>
#include <string>
#include <string_view>

namespace mentor::tui
{

namespace detail
{

inline int utf8CharLen(unsigned char c) noexcept
{
    if ((c & 0x80) == 0x00)
        return 1;
    if ((c & 0xE0) == 0xC0)
        return 2;
    if ((c & 0xF0) == 0xE0)
        return 3;
    if ((c & 0xF8) == 0xF0)
        return 4;
    return 1;
}

inline int utf8DisplayWidth(std::string_view ch) noexcept
{
    if (ch.size() == 1)
        return 1;
    if (ch.size() >= 3)
        return 2;
    return 1;
}

} // namespace detail

inline void insertParagragh(std::string_view text,
                            int max_width,
                            ftxui::Elements &body,
                            ftxui::Decorator decorator = nullptr) noexcept
{
    if (max_width < 1)
        max_width = 1;

    auto pushLine = [&](const std::string &line)
    {
        if (decorator)
            body.push_back(ftxui::text(line) | decorator);
        else
            body.push_back(ftxui::text(line));
    };

    std::string current_line;
    int current_width = 0;

    const size_t size = text.size();
    size_t i          = 0;

    while (i < size)
    {
        unsigned char c = static_cast<unsigned char>(text[i]);
        int char_len    = detail::utf8CharLen(c);

        if (i + static_cast<size_t>(char_len) > size)
        {
            char_len = 1;
        }
        else if (char_len > 1)
        {
            bool valid = true;
            for (int k = 1; k < char_len; ++k)
            {
                unsigned char cc = static_cast<unsigned char>(text[i + k]);
                if ((cc & 0xC0) != 0x80)
                {
                    valid = false;
                    break;
                }
            }
            if (!valid)
                char_len = 1;
        }

        std::string ch(text.substr(i, static_cast<size_t>(char_len)));
        int display_w = detail::utf8DisplayWidth(ch);

        if (ch == "\n")
        {
            pushLine(current_line);
            current_line.clear();
            current_width = 0;
            i += static_cast<size_t>(char_len);
            continue;
        }

        if (current_width + display_w > max_width && !current_line.empty())
        {
            pushLine(current_line);
            current_line  = ch;
            current_width = display_w;
        }
        else
        {
            current_line += ch;
            current_width += display_w;
        }

        i += static_cast<size_t>(char_len);
    }

    if (!current_line.empty())
        pushLine(current_line);
}

} // namespace mentor::tui
