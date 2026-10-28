#include "Kernel/Kernel.hpp"
#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Event/LoadTestFile.hpp"
#include "Codec/TestFile.hpp"

namespace mentor::kernel
{

Response Kernel::handle(Event e)
{
    switch (e.type)
    {
    case EventType::error:
        return {ResponseType::error};
    case EventType::load_test_file:
        return handleLoadTestFile(e);
    }
}

Response Kernel::handleLoadTestFile(Event e)
{
    auto payload     = static_cast<event::LoadTestFile *>(e.payload);
    const auto &file = *m_fs.getJsonFile(payload->fid);
    auto decoded     = codec::decode<TestFile>(file);
    if (!decoded.has_value())
        return {ResponseType::load_file_failed};
    m_test_file = std::move(decoded.value());
    return {ResponseType::load_file_succeed};
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
