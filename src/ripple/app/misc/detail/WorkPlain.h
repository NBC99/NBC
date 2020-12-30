

#ifndef RIPPLE_APP_MISC_DETAIL_WORKPLAIN_H_INCLUDED
#define RIPPLE_APP_MISC_DETAIL_WORKPLAIN_H_INCLUDED

#include <ripple/app/misc/detail/WorkBase.h>

namespace ripple {

namespace detail {

class WorkPlain : public WorkBase<WorkPlain>
    , public std::enable_shared_from_this<WorkPlain>
{
    friend class WorkBase<WorkPlain>;

public:
    WorkPlain(
        std::string const& host,
        std::string const& path, std::string const& port,
        boost::asio::io_service& ios, callback_type cb);
    ~WorkPlain() = default;

private:
    void
    onConnect(error_code const& ec);

    socket_type&
    stream()
    {
        return socket_;
    }
};


WorkPlain::WorkPlain(
    std::string const& host,
    std::string const& path, std::string const& port,
    boost::asio::io_service& ios, callback_type cb)
    : WorkBase (host, path, port, ios, cb)
{
}

void
WorkPlain::onConnect(error_code const& ec)
{
    if (ec)
        return fail(ec);

    onStart ();
}

} 

} 

#endif








