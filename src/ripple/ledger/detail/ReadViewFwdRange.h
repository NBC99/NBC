

#ifndef RIPPLE_LEDGER_READVIEWFWDRANGE_H_INCLUDED
#define RIPPLE_LEDGER_READVIEWFWDRANGE_H_INCLUDED

#include <cstddef>
#include <iterator>
#include <memory>
#include <boost/optional.hpp>

namespace ripple {

class ReadView;

namespace detail {

template <class ValueType>
class ReadViewFwdIter
{
public:
    using base_type = ReadViewFwdIter;

    using value_type = ValueType;

    ReadViewFwdIter() = default;
    ReadViewFwdIter(ReadViewFwdIter const&) = default;
    ReadViewFwdIter& operator=(ReadViewFwdIter const&) = default;

    virtual
    ~ReadViewFwdIter() = default;

    virtual
    std::unique_ptr<ReadViewFwdIter>
    copy() const = 0;

    virtual
    bool
    equal (ReadViewFwdIter const& impl) const = 0;

    virtual
    void
    increment() = 0;

    virtual
    value_type
    dereference() const = 0;
};

template<class ValueType>
class ReadViewFwdRange
{
public:
    using iter_base =
        ReadViewFwdIter<ValueType>;

   static_assert(
        std::is_nothrow_move_constructible<ValueType>{},
        "ReadViewFwdRange move and move assign constructors should be "
        "noexcept");

    class iterator
    {
    public:
        using value_type = ValueType;

        using pointer = value_type const*;

        using reference = value_type const&;

        using difference_type =
            std::ptrdiff_t;

        using iterator_category =
            std::forward_iterator_tag;

        iterator() = default;

        iterator (iterator const& other);
        iterator (iterator&& other) noexcept;

        explicit
        iterator (ReadView const* view,
            std::unique_ptr<iter_base> impl);

        iterator&
        operator= (iterator const& other);

        iterator&
        operator= (iterator&& other) noexcept;

        bool
        operator== (iterator const& other) const;

        bool
        operator!= (iterator const& other) const;

        reference
        operator*() const;

        pointer
        operator->() const;

        iterator&
        operator++();

        iterator
        operator++(int);

    private:
        ReadView const* view_ = nullptr;
        std::unique_ptr<iter_base> impl_;
        boost::optional<value_type> mutable cache_;
    };

    static_assert(std::is_nothrow_move_constructible<iterator>{}, "");
    static_assert(std::is_nothrow_move_assignable<iterator>{}, "");
 
    using const_iterator = iterator;

    using value_type = ValueType;

    ReadViewFwdRange() = delete;
    ReadViewFwdRange (ReadViewFwdRange const&) = default;
    ReadViewFwdRange& operator= (ReadViewFwdRange const&) = default;


    explicit
    ReadViewFwdRange (ReadView const& view)
        : view_ (&view)
    {
    }

protected:
    ReadView const* view_;
    boost::optional<iterator> mutable end_;
};

} 
} 

#endif








