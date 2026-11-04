#pragma once

#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Base/Protocol/TestFile.hpp"
#include "Kernel/State.hpp"
#include "Service/FileService.hpp"
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

private:
    Response handleLoadTestFile(Event) noexcept;
    Response handleRequestQuestion(Event) noexcept;

private:
    Response cacheChoiceQuestion(question_index) noexcept;
    Response cacheEssayQuestion(question_index) noexcept;

private:
    TestFile m_test_file;
    std::unordered_map<question_index, QuestionHandle> m_cache;
    std::vector<ChoiceQuestion> m_choice_questions;
    std::vector<EssayQuestion> m_essay_questions;
    service::FileService &m_fs;
    State m_state = State::init;
};

} // namespace mentor::kernel
