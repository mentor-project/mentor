#pragma once

#include "Base/Event/Event.hpp"
#include "Base/Event/Response.hpp"
#include "Base/Protocol/TestFile.hpp"
#include "Kernel/State.hpp"
#include "Service/FileService.hpp"

namespace mentor::kernel
{

class Kernel
{
public:
    Kernel(service::FileService &fs) : m_fs(fs) {}
    Response handle(Event e);
    State getState() const noexcept;
#ifdef MENTOR_LIB_TEST
    void setState(State s) noexcept;
#endif

private:
    Response handleLoadTestFile(Event e);

private:
    TestFile m_test_file;
    service::FileService &m_fs;
    State m_state = State::Init;
};

} // namespace mentor::kernel
