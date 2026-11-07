#pragma once

#include <tuple>
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
    Payload(Payload &&p) noexcept
    {
        moveImpl(p);
    }
    Payload &operator=(Payload &&p)
    {
        moveImpl(p);
        return *this;
    }
    ~Payload()
    {
        if (m_deleter)
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

    static Payload makeView(void *p) noexcept
    {
        return p;
    }

    template <typename T, typename... Args>
    static Payload makeOwned(void (*deleter)(void *), Args &&...args)
    {
        return {new T(std::forward<Args>(args)...), deleter};
    }

    template <typename T, typename... Args>
    static Payload makeOwned(Args &&...args) noexcept
    {
        if constexpr (sizeof...(Args) == 0)
        {
            return {new T(), [](void *x)
                    {
                        delete static_cast<T *>(x);
                    }};
        }
        else if constexpr (LastIsDeleter<Args...>)
        {
            return [&]<std::size_t... I>(std::index_sequence<I...>) -> Payload
            {
                constexpr std::size_t N = sizeof...(I);
                auto t                  = std::forward_as_tuple(std::forward<Args>(args)...);
                void (*deleter)(void *) = std::get<N>(t);
                return {new T(std::get<I>(t)...), deleter};
            }(std::make_index_sequence<sizeof...(Args) - 1>{});
        }
        else
        {
            return {new T(std::forward<Args>(args)...),
                    [](void *x)
                    {
                        delete static_cast<T *>(x);
                    }};
        }
    }

private:
    template <typename... Args>
    static constexpr bool LastIsDeleter =
        std::is_convertible_v<
            std::tuple_element_t<sizeof...(Args) - 1, std::tuple<Args...>>,
            void (*)(void *)>;

    void moveImpl(Payload &p) noexcept
    {
        m_ptr       = p.m_ptr;
        m_deleter   = p.m_deleter;
        p.m_ptr     = nullptr;
        p.m_deleter = nullptr;
    }

private:
    void *m_ptr;
    void (*m_deleter)(void *);
};

} // namespace mentor
