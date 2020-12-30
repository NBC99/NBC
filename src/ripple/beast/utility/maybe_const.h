

#ifndef BEAST_UTILITY_MAYBE_CONST_H_INCLUDED
#define BEAST_UTILITY_MAYBE_CONST_H_INCLUDED

#include <type_traits>

namespace beast {


template <bool IsConst, class T>
struct maybe_const
{
    explicit maybe_const() = default;
    using type = typename std::conditional <IsConst,
        typename std::remove_const <T>::type const,
        typename std::remove_const <T>::type>::type;
};


template <bool IsConst, class T>
using maybe_const_t = typename maybe_const <IsConst,T>::type;

}

#endif








