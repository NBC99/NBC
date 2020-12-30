

#ifndef RIPPLE_PEERFINDER_IOSFORMAT_H_INCLUDED
#define RIPPLE_PEERFINDER_IOSFORMAT_H_INCLUDED

#include <ostream>
#include <sstream>
#include <string>

namespace beast {



struct leftw
{
    explicit leftw (int width_)
        : width (width_)
        { }
    int const width;
    template <class CharT, class Traits>
    friend std::basic_ios <CharT, Traits>& operator<< (
        std::basic_ios <CharT, Traits>& ios, leftw const& p)
    {
        ios.setf (std::ios_base::left, std::ios_base::adjustfield);
        ios.width (p.width);
        return ios;
    }
};


template <class CharT, class Traits, class Allocator>
std::basic_string <CharT, Traits, Allocator> heading (
    std::basic_string <CharT, Traits, Allocator> title,
        int width = 80, CharT fill = CharT ('-'))
{
    title.reserve (width);
    title.push_back (CharT (' '));
    title.resize (width, fill);
    return title;
}


struct divider
{
    using CharT = char;
    explicit divider (int width_ = 80, CharT fill_ = CharT ('-'))
        : width (width_)
        , fill (fill_)
        { }
    int const width;
    CharT const fill;
    template <class CharT, class Traits>
    friend std::basic_ostream <CharT, Traits>& operator<< (
        std::basic_ostream <CharT, Traits>& os, divider const& d)
    {
        os << std::basic_string <CharT, Traits> (d.width, d.fill);
        return os;
    }
};


struct fpad
{
    explicit fpad (int width_, int pad_ = 0, char fill_ = ' ')
        : width (width_ + pad_)
        , fill (fill_)
        { }
    int const width;
    char const fill;
    template <class CharT, class Traits>
    friend std::basic_ostream <CharT, Traits>& operator<< (
        std::basic_ostream <CharT, Traits>& os, fpad const& f)
    {
        os << std::basic_string <CharT, Traits> (f.width, f.fill);
        return os;
    }
};


namespace detail {

template <typename T>
std::string to_string (T const& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

}



template <class CharT,
          class Traits = std::char_traits <CharT>,
          class Allocator = std::allocator <CharT>>
class field_t
{
public:
    using string_t = std::basic_string <CharT, Traits, Allocator>;
    field_t (string_t const& text_, int width_, int pad_, bool right_)
        : text (text_)
        , width (width_)
        , pad (pad_)
        , right (right_)
        { }
    string_t const text;
    int const width;
    int const pad;
    bool const right;
    template <class CharT2, class Traits2>
    friend std::basic_ostream <CharT2, Traits2>& operator<< (
        std::basic_ostream <CharT2, Traits2>& os,
            field_t <CharT, Traits, Allocator> const& f)
    {
        std::size_t const length (f.text.length());
        if (f.right)
        {
            if (length < f.width)
                os << std::basic_string <CharT2, Traits2> (
                    f.width - length, CharT2 (' '));
            os << f.text;
        }
        else
        {
            os << f.text;
            if (length < f.width)
                os << std::basic_string <CharT2, Traits2> (
                    f.width - length, CharT2 (' '));
        }
        if (f.pad != 0)
            os << string_t (f.pad, CharT (' '));
        return os;
    }
};

template <class CharT, class Traits, class Allocator>
field_t <CharT, Traits, Allocator> field (
    std::basic_string <CharT, Traits, Allocator> const& text,
        int width = 8, int pad = 0, bool right = false)
{
    return field_t <CharT, Traits, Allocator> (
        text, width, pad, right);
}

template <class CharT>
field_t <CharT> field (
    CharT const* text, int width = 8, int pad = 0, bool right = false)
{
    return field_t <CharT, std::char_traits <CharT>,
        std::allocator <CharT>> (std::basic_string <CharT,
            std::char_traits <CharT>, std::allocator <CharT>> (text),
                width, pad, right);
}

template <typename T>
field_t <char> field (
    T const& t, int width = 8, int pad = 0, bool right = false)
{
    std::string const text (detail::to_string (t));
    return field (text, width, pad, right);
}

template <class CharT, class Traits, class Allocator>
field_t <CharT, Traits, Allocator> rfield (
    std::basic_string <CharT, Traits, Allocator> const& text,
        int width = 8, int pad = 0)
{
    return field_t <CharT, Traits, Allocator> (
        text, width, pad, true);
}

template <class CharT>
field_t <CharT> rfield (
    CharT const* text, int width = 8, int pad = 0)
{
    return field_t <CharT, std::char_traits <CharT>,
        std::allocator <CharT>> (std::basic_string <CharT,
            std::char_traits <CharT>, std::allocator <CharT>> (text),
                width, pad, true);
}

template <typename T>
field_t <char> rfield (
    T const& t, int width = 8, int pad = 0)
{
    std::string const text (detail::to_string (t));
    return field (text, width, pad, true);
}


}

#endif








