#pragma once

#include <utility>
namespace mentor
{

class Payload
{
public:
    Payload() : Payload(nullptr, nullptr) {}
    Payload(void *p) : Payload(p, nullptr) {}
    Payload(void *p, void (*deleter)(void *)) : m_ptr(p), m_deleter(deleter) {}
    Payload(const Payload &)            = delete;
    Payload &operator=(const Payload &) = delete;
    ~Payload()
    {
        if (m_deleter && m_ptr)
            m_deleter(m_ptr);
    }

    bool isView() const noexcept
    {
        return !m_deleter;
    }

    bool isOwner() const noexcept
    {
        return m_deleter;
    }

    template <typename T>
    T get() const noexcept
    {
        return static_cast<T>(m_ptr);
    }

    static Payload makeView(void *p){
        return p;
    }

    template <typename T, typename... Args>
    static Payload makeOwned(Args &&...args)
    {
        return {new T(std::forward<Args>(args)...), [](void *x)
                {
                    delete static_cast<T *>(x);
                }};
    }

private:
    void *m_ptr;
    void (*m_deleter)(void *);
};

} // namespace mentor
