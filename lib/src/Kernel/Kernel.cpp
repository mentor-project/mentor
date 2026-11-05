#include "Kernel/Kernel.hpp"
#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Codec/Model.hpp"
#include "Codec/TestFile.hpp"
#include "Event/LoadTestFile.hpp"
#include "Event/RequestQuestion.hpp"
#include "Event/SubmitAnswer.hpp"
#include "Response/CheckResult.hpp"
#include "Kernel/State.hpp"
#include <cstdint>

namespace mentor::kernel
{

Response Kernel::handle(Event e) noexcept
{
    switch (e.type)
    {
    case EventType::load_test_file:
        return handleLoadTestFile(e);
    case EventType::request_question:
        return handleRequestQuestion(e);
    case EventType::submit_answer:
        return handleSubmitAnswer(e);
    default:
        break;
    }

    return Response::error();
}

Response Kernel::handleLoadTestFile(Event e) noexcept
{
    if (m_state != State::init)
        return Response::error();

    auto payload     = static_cast<event::LoadTestFile *>(e.payload);
    const auto &file = *m_fs.getJsonFile(payload->fid);
    auto decoded     = codec::decode<TestFile>(file);
    if (!decoded.has_value())
        return {ResponseType::load_file_failed};
    m_test_file                     = std::move(decoded.value());
    m_state                         = State::testing;
    m_test_file.info.question_count = m_test_file.questions.size();
    return {ResponseType::load_file_succeed, &m_test_file.info};
}

Response Kernel::handleRequestQuestion(Event e) noexcept
{
    if (m_state != State::testing)
        return Response::error();

    auto payload = static_cast<event::RequestQuestion *>(e.payload);
    auto i       = payload->qi;

    if (i >= m_test_file.questions.size())
        return {ResponseType::post_question_failed};

    // cache hit
    if (m_cache.contains(i))
    {
        auto &handle = m_cache.at(i);
        switch (handle.type)
        {
        case QuestionType::choice:
            return {ResponseType::post_question_succeed, &handle};
        case QuestionType::essay:
            return {ResponseType::post_question_succeed, &handle};
        default:
            break;
        }
        return {ResponseType::post_question_failed};
    }

    auto &questions = m_test_file.questions;
    auto type       = questions[i]["type"].get<QuestionType>();
    switch (type)
    {
    case QuestionType::choice:
        return cacheChoiceQuestion(i);
    case QuestionType::essay:
        return cacheEssayQuestion(i);
    default:
        break;
    }

    return {ResponseType::post_question_failed};
}

Response Kernel::handleSubmitAnswer(Event e) noexcept
{
    if (m_state != State::testing)
        return Response::error();

    auto payload = static_cast<event::SubmitAnswer *>(e.payload);

    if (is<ChoiceQuestion>(payload->qi))
        return checkChoiceQuestion(m_cache.at(payload->qi).index, payload->choice);
    else if (is<EssayQuestion>(payload->qi))
        return checkEssayQuestion(m_cache.at(payload->qi).index, payload->li, payload->line);

    return Response::error();
}

Response Kernel::cacheChoiceQuestion(question_index i) noexcept
{
    auto q = codec::decode<ChoiceQuestion>(m_test_file.questions[i]);
    if (!q)
        return {ResponseType::post_question_failed};

    QuestionHandle handle;
    handle.index = m_choice_questions.size();
    handle.type  = QuestionType::choice;

    m_choice_questions.push_back(std::move(*q));
    auto [it, _] = m_cache.try_emplace(i, handle);
    return {ResponseType::post_question_succeed, &it->second};
}

Response Kernel::cacheEssayQuestion(question_index i) noexcept
{
    auto q = codec::decode<EssayQuestion>(m_test_file.questions[i]);
    if (!q)
        return {ResponseType::post_question_failed};

    QuestionHandle handle;
    handle.index = m_essay_questions.size();
    handle.type  = QuestionType::essay;

    m_essay_questions.push_back(std::move(*q));
    auto [it, _] = m_cache.try_emplace(i, handle);
    return {ResponseType::post_question_succeed, &it->second};
}

Response Kernel::checkChoiceQuestion(question_index qi, uint32_t choice) noexcept
{
    auto &q = getQuestion<ChoiceQuestion>(qi);

    if (choice == q.answer)
        return {ResponseType::correct};

    std::string answer;
    answer.reserve(16);
    for (int i = 0; i < 32; ++i)
    {
        if (q.answer & (1u << i))
        {
            if (!answer.empty())
                answer += "、";
            answer += static_cast<char>('A' + i);
        }
    }

    if (choice & ~q.answer)
        return {ResponseType::wrong, Payload::makeOwned<response::CheckResult>(std::move(answer), 0.0f)};

    uint32_t answerCount   = std::popcount(q.answer);
    uint32_t chosenCorrect = std::popcount(choice);
    double ratio      = answerCount > 0
                            ? static_cast<float>(chosenCorrect) / answerCount
                            : 0.0;

    return {ResponseType::wrong, Payload::makeOwned<response::CheckResult>(std::move(answer), ratio)};
}

Response Kernel::checkEssayQuestion(question_index qi, uint32_t li, std::string_view line) noexcept
{
    return {ResponseType::wrong};
}

State Kernel::getState() const noexcept
{
    return m_state;
}

#ifdef MENTOR_LIB_TEST
void Kernel::setState(State s) noexcept
{
    m_state = s;
}
#endif

} // namespace mentor::kernel
