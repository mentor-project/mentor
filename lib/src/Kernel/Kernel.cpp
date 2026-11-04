#include "Kernel/Kernel.hpp"
#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/Model.hpp"
#include "Codec/Model.hpp"
#include "Codec/TestFile.hpp"
#include "Event/LoadTestFile.hpp"
#include "Event/RequestQuestion.hpp"
#include "Kernel/State.hpp"

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
    m_test_file = std::move(decoded.value());
    m_state     = State::testing;
    Response response;
    response.type = ResponseType::load_file_succeed;
    m_test_file.info.question_count = m_test_file.questions.size();
    response.payload = &m_test_file.info;
    return response;
}

Response Kernel::handleRequestQuestion(Event e) noexcept
{
    if (m_state != State::testing)
        return Response::error();

    auto payload = static_cast<event::RequestQuestion *>(e.payload);
    auto i       = payload->qi;

    // cache hit
    if (m_cache.contains(i))
    {
        auto [index, type] = m_cache.at(i);
        switch (type)
        {
        case QuestionType::choice:
            return {ResponseType::post_choice_question, &m_choice_questions[index]};
        case QuestionType::essay:
            return {ResponseType::post_essay_question, &m_essay_questions[index]};
        default:
            break;
        }
        return Response::error();
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

    return Response::error();
}

Response Kernel::cacheChoiceQuestion(question_index i) noexcept
{
    Response response;
    response.type = ResponseType::post_choice_question;

    auto q = codec::decode<ChoiceQuestion>(m_test_file.questions[i]);
    if (!q)
        return Response::error();

    QuestionHandle handle;
    handle.index = m_choice_questions.size();
    handle.type  = QuestionType::choice;

    m_choice_questions.push_back(std::move(*q));
    m_cache.emplace(i, handle);
    response.payload = &m_choice_questions.back();
    return response;
}

Response Kernel::cacheEssayQuestion(question_index i) noexcept
{
    Response response;
    response.type = ResponseType::post_essay_question;

    auto q = codec::decode<EssayQuestion>(m_test_file.questions[i]);
    if (!q)
        return Response::error();

    QuestionHandle handle;
    handle.index = m_essay_questions.size();
    handle.type  = QuestionType::essay;

    m_essay_questions.push_back(std::move(*q));
    m_cache.emplace(i, handle);
    response.payload = &m_essay_questions.back();
    return response;
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
