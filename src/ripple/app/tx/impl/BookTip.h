

#ifndef RIPPLE_APP_BOOK_BOOKTIP_H_INCLUDED
#define RIPPLE_APP_BOOK_BOOKTIP_H_INCLUDED

#include <ripple/protocol/Quality.h>
#include <ripple/ledger/View.h>
#include <ripple/protocol/Indexes.h>

#include <functional>

namespace ripple {

class Logs;


class BookTip
{
private:
    ApplyView& view_;
    bool m_valid;
    uint256 m_book;
    uint256 m_end;
    uint256 m_dir;
    uint256 m_index;
    std::shared_ptr<SLE> m_entry;
    Quality m_quality;

public:
    
    BookTip (ApplyView& view, Book const& book);

    uint256 const&
    dir() const noexcept
    {
        return m_dir;
    }

    uint256 const&
    index() const noexcept
    {
        return m_index;
    }

    Quality const&
    quality() const noexcept
    {
        return m_quality;
    }

    SLE::pointer const&
    entry() const noexcept
    {
        return m_entry;
    }

    
    bool
    step (beast::Journal j);
};

}

#endif








