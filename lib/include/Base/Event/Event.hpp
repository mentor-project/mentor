#pragma once

namespace mentor
{

enum class EventType
{
    error,
    load_test_file
};

struct Event
{
    EventType type;
    void *payload;
};

template <typename Ptr>
Event toEvent(EventType type, Ptr &&payload)
{
    return {type, payload.get()};
}

} // namespace mentor
