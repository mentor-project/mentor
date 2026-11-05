#pragma once

#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Base/Protocol/TestFile.hpp"
#include "Kernel/State.hpp"
#include "Service/FileService.hpp"
#include <concepts>
#include <unordered_map>

namespace mentor::kernel
{

using question_index = uint32_t;

struct QuestionHandle
{
    question_index index;
    QuestionType type;
};

class Kernel
{
public:
    Kernel(service::FileService &fs) : m_fs(fs) {}
    Response handle(Event) noexcept;
    State getState() const noexcept;
#ifdef MENTOR_LIB_TEST
    void setState(State) noexcept;
#endif

public:
    template <typename Question>
    const Question *tryGetQuestion(QuestionHandle handle) const noexcept
        requires std::same_as<Question, ChoiceQuestion>
    {
        if (!is<Question>(handle))
            return nullptr;

        return &m_choice_questions[handle.index];
    }

    template <typename Question>
    const Question *tryGetQuestion(QuestionHandle handle) const noexcept
        requires std::same_as<Question, EssayQuestion>
    {
        if (!is<Question>(handle))
            return nullptr;

        return &m_essay_questions[handle.index];
    }

    template <typename Question>
    bool is(QuestionHandle handle) const noexcept
        requires std::same_as<Question, ChoiceQuestion>
    {
        return handle.type == QuestionType::choice;
    }

    template <typename Question>
    bool is(QuestionHandle handle) const noexcept
        requires std::same_as<Question, EssayQuestion>
    {
        return handle.type == QuestionType::essay;
    }

private:
    template <typename Question>
    const Question &getQuestion(question_index i) const noexcept
        requires std::same_as<Question, ChoiceQuestion>
    {
        return m_choice_questions[i];
    }

    template <typename Question>
    const Question &getQuestion(question_index i) const noexcept
        requires std::same_as<Question, EssayQuestion>
    {
        return m_essay_questions[i];
    }

    template <typename Question>
    bool is(question_index i) const noexcept
        requires std::same_as<Question, ChoiceQuestion>
    {
        return m_cache.at(i).type == QuestionType::choice;
    }

    template <typename Question>
    bool is(question_index i) const noexcept
        requires std::same_as<Question, EssayQuestion>
    {
        return m_cache.at(i).type == QuestionType::essay;
    }

private:
    Response handleLoadTestFile(Event) noexcept;
    Response handleRequestQuestion(Event) noexcept;
    Response handleSubmitAnswer(Event) noexcept;

private:
    Response cacheChoiceQuestion(question_index) noexcept;
    Response cacheEssayQuestion(question_index) noexcept;

private:
    Response checkChoiceQuestion(question_index, uint32_t) noexcept;
    Response checkEssayQuestion(question_index, uint32_t, std::string_view) noexcept;

private:
    TestFile m_test_file;
    std::unordered_map<question_index, QuestionHandle> m_cache;
    std::vector<ChoiceQuestion> m_choice_questions;
    std::vector<EssayQuestion> m_essay_questions;
    service::FileService &m_fs;
    State m_state = State::init;
};

} // namespace mentor::kernel
