

#ifndef BEAST_CONTAINER_AGED_UNORDERED_MULTIMAP_H_INCLUDED
#define BEAST_CONTAINER_AGED_UNORDERED_MULTIMAP_H_INCLUDED

#include <ripple/beast/container/detail/aged_unordered_container.h>

#include <chrono>
#include <functional>
#include <memory>

namespace beast {

template <
    class Key,
    class T,
    class Clock = std::chrono::steady_clock,
    class Hash = std::hash <Key>,
    class KeyEqual = std::equal_to <Key>,
    class Allocator = std::allocator <std::pair <Key const, T>>
>
using aged_unordered_multimap = detail::aged_unordered_container <
    true, true, Key, T, Clock, Hash, KeyEqual, Allocator>;

}

#endif








