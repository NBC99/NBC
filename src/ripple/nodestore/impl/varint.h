

#ifndef BEAST_NUDB_VARINT_H_INCLUDED
#define BEAST_NUDB_VARINT_H_INCLUDED

#include <nudb/detail/stream.hpp>
#include <cstdint>
#include <type_traits>

namespace ripple {
namespace NodeStore {


struct varint;

template <class T,
    bool = std::is_unsigned<T>::value>
struct varint_traits;

template <class T>
struct varint_traits<T, true>
{
    explicit varint_traits() = default;

    static std::size_t constexpr max =
        (8 * sizeof(T) + 6) / 7;
};

template <class = void>
std::size_t
read_varint (void const* buf,
    std::size_t buflen, std::size_t& t)
{
    t = 0;
    std::uint8_t const* p =
        reinterpret_cast<
            std::uint8_t const*>(buf);
    std::size_t n = 0;
    while (p[n] & 0x80)
        if (++n >= buflen)
            return 0;
    if (++n > buflen)
        return 0;
    if (n == 1 && *p == 0)
    {
        t = 0;
        return 1;
    }
    auto const used = n;
    while (n--)
    {
        auto const d = p[n];
        auto const t0 = t;
        t *= 127;
        t += d & 0x7f;
        if (t <= t0)
            return 0; 
    }
    return used;
}

template <class T,
    std::enable_if_t<std::is_unsigned<
        T>::value>* = nullptr>
std::size_t
size_varint (T v)
{
    std::size_t n = 0;
    do
    {
        v /= 127;
        ++n;
    }
    while (v != 0);
    return n;
}

template <class = void>
std::size_t
write_varint (void* p0, std::size_t v)
{
    std::uint8_t* p = reinterpret_cast<
        std::uint8_t*>(p0);
    do
    {
        std::uint8_t d =
            v % 127;
        v /= 127;
        if (v != 0)
            d |= 0x80;
        *p++ = d;
    }
    while (v != 0);
    return p - reinterpret_cast<
        std::uint8_t*>(p0);
}


template <class T, std::enable_if_t<
    std::is_same<T, varint>::value>* = nullptr>
void
read (nudb::detail::istream& is, std::size_t& u)
{
    auto p0 = is(1);
    auto p1 = p0;
    while (*p1++ & 0x80)
        is(1);
    read_varint(p0, p1 - p0, u);
}


template <class T, std::enable_if_t<
    std::is_same<T, varint>::value>* = nullptr>
void
write (nudb::detail::ostream& os, std::size_t t)
{
    write_varint(os.data(
        size_varint(t)), t);
}

} 
} 

#endif








