#pragma once

namespace mentor
{

enum class EventType
{
    error,
    load_test_file,
    request_question,
};

struct Event
{
    EventType type;
    void *payload{};

    // we do not put payload when meeting error for now
    static Event error() noexcept
    {
        return {EventType::error};
    }
};

template <typename Ptr>
Event toEvent(EventType type, Ptr &&payload)
{
    return {type, payload.get()};
}

} // namespace mentor
