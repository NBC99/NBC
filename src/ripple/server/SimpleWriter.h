

#ifndef RIPPLE_SERVER_SIMPLEWRITER_H_INCLUDED
#define RIPPLE_SERVER_SIMPLEWRITER_H_INCLUDED

#include <ripple/server/Writer.h>
#include <boost/beast/core/multi_buffer.hpp>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/write.hpp>
#include <utility>

namespace ripple {

class SimpleWriter : public Writer
{
    boost::beast::multi_buffer sb_;

public:
    template<bool isRequest, class Body, class Fields>
    explicit
    SimpleWriter(boost::beast::http::message<isRequest, Body, Fields> const& msg)
    {
        boost::beast::ostream(sb_) << msg;
    }

    bool
    complete() override
    {
        return sb_.size() == 0;
    }

    void
    consume (std::size_t bytes) override
    {
        sb_.consume(bytes);
    }

    bool
    prepare(std::size_t bytes,
        std::function<void(void)>) override
    {
        return true;
    }

    std::vector<boost::asio::const_buffer>
    data() override
    {
        auto const& buf = sb_.data();
        std::vector<boost::asio::const_buffer> result;
        result.reserve(std::distance(buf.begin(), buf.end()));
        for (auto const& b : buf)
            result.push_back(b);
        return result;
    }
};

} 

#endif








