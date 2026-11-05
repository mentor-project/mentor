#pragma once

#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Basic/AppContext.hpp"
#include "Event/SubmitAnswer.hpp"
#include "Helper/Paragragh.hpp"
#include "Response/CheckResult.hpp"
#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

namespace mentor::tui
{

class ChoiceQuestionPage : public ftxui::ComponentBase
{
public:
    explicit ChoiceQuestionPage(AppContext &ac)
        : m_ac(ac)
    {
    }

    ftxui::Element OnRender() override
    {
        using namespace ftxui;

        loadOrRefreshQuestion();

        if (!m_q)
            return vbox();

        constexpr int width = 72;

        Elements body;
        insertParagragh(m_q->stem, width - 2, body, bold);
        if (!m_q->hint.empty())
            insertParagragh(m_q->hint, width - 2, body, dim);
        body.push_back(separator());

        body.push_back(hbox({
            text(m_q->isMultiple() ? "【多选题】" : "【单选题】") | color(Color::Yellow),
            text("   分值 " + std::to_string(m_q->score)) | dim,
        }));

        body.push_back(m_inner->Render() | frame);

        body.push_back(separator());

        if (m_correctness >= 0.0f)
        {
            body.push_back(m_correctness == 1.0f
                               ? text("✓ 回答正确") | color(Color::Green) | bold
                               : text("✗ 回答错误") | color(Color::Red) | bold);
            if (m_correctness != 1.0f)
                body.push_back(text("正确答案：" +m_answer));
            if (!m_q->solution.empty())
            {
                body.push_back(separator());
                body.push_back(text("解析") | bold);
                insertParagragh(m_q->solution, width - 2, body, dim);
            }
        }
        else
        {
            body.push_back(text("↑↓ 移动，Space 勾选，Enter 提交") | dim);
        }

        return vbox(std::move(body)) | border | size(WIDTH, EQUAL, width) | center;
    }

    bool OnEvent(ftxui::Event e) override
    {
        using namespace ftxui;

        loadOrRefreshQuestion();

        if (!m_q)
            return false;

        if (e == ftxui::Event::Return)
        {
            auto payload  = std::make_unique<event::SubmitAnswer>(m_ac.cur_question_idx, getChosen());
            auto response = m_ac.kernel.handle(toEvent(EventType::submit_answer, payload));
            if (response.type == ResponseType::correct)
            {
                m_correctness = 1.0f;
                return true;
            }
            else if (response.type == ResponseType::wrong)
            {
                auto result   = response.payload.get<response::CheckResult *>();
                m_correctness = result->correctness;
                m_answer      = std::move(result->answer);
                return true;
            }
            return true;
        }

        return m_inner->OnEvent(e);
    }

private:
    void loadOrRefreshQuestion() noexcept
    {
        auto *q = m_ac.kernel.tryGetQuestion<ChoiceQuestion>(m_ac.cur_question);

        if (!q || q == m_q)
            return;

        m_q           = q;
        m_correctness = -1.0;
        m_check_states.clear();

        if (m_q->isMultiple())
        {
            m_check_states.assign(m_q->options.size(), false);
            std::vector<ftxui::Component> boxes;
            boxes.reserve(m_q->options.size());
            for (size_t i = 0; i < m_q->options.size(); ++i)
                boxes.push_back(ftxui::Checkbox(&m_q->options[i], &m_check_states[i]));
            m_inner = ftxui::Container::Vertical(std::move(boxes));
        }
        else
        {
            m_single_selected = 0;
            m_inner           = ftxui::Radiobox(&m_q->options, &m_single_selected);
        }
    }

    uint32_t getChosen() const noexcept
    {
        if (!m_q->isMultiple())
            return 1u << m_single_selected;

        uint32_t chosen = 0;
        auto size       = m_q->options.size();
        for (size_t i = 0; i < size; ++i)
        {
            if (m_check_states[i])
                chosen |= (1u << i);
        }
        return chosen;
    }

private:
    std::string m_answer;
    std::deque<bool> m_check_states;
    ftxui::Component m_inner;
    AppContext &m_ac;
    const ChoiceQuestion *m_q{};
    int m_single_selected{};
    float m_correctness{-1.0f};
};

inline ftxui::Component getChoiceQuestionPage(AppContext &ac)
{
    return ftxui::Make<ChoiceQuestionPage>(ac);
}

} // namespace mentor::tui
