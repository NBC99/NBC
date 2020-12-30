

#include <ripple/net/RPCSub.h>
#include <ripple/basics/contract.h>
#include <ripple/basics/Log.h>
#include <ripple/basics/StringUtilities.h>
#include <ripple/json/to_string.h>
#include <ripple/net/RPCCall.h>
#include <deque>

namespace ripple {

class RPCSubImp
    : public RPCSub
{
public:
    RPCSubImp (InfoSub::Source& source, boost::asio::io_service& io_service,
        JobQueue& jobQueue, std::string const& strUrl, std::string const& strUsername,
             std::string const& strPassword, Logs& logs)
        : RPCSub (source)
        , m_io_service (io_service)
        , m_jobQueue (jobQueue)
        , mUrl (strUrl)
        , mSSL (false)
        , mUsername (strUsername)
        , mPassword (strPassword)
        , mSending (false)
        , j_ (logs.journal ("RPCSub"))
        , logs_ (logs)
    {
        parsedURL pUrl;

        if (!parseUrl (pUrl, strUrl))
            Throw<std::runtime_error> ("Failed to parse url.");
        else if (pUrl.scheme == "https")
            mSSL = true;
        else if (pUrl.scheme != "http")
            Throw<std::runtime_error> ("Only http and https is supported.");

        mSeq = 1;

        mIp = pUrl.domain;
        mPort = (! pUrl.port) ? (mSSL ? 443 : 80) : *pUrl.port;
        mPath = pUrl.path;

        JLOG (j_.info()) <<
            "RPCCall::fromNetwork sub: ip=" << mIp <<
            " port=" << mPort <<
            " ssl= "<< (mSSL ? "yes" : "no") <<
            " path='" << mPath << "'";
    }

    ~RPCSubImp() = default;

    void send (Json::Value const& jvObj, bool broadcast) override
    {
        ScopedLockType sl (mLock);

        if (mDeque.size () >= eventQueueMax)
        {
            JLOG (j_.warn()) << "RPCCall::fromNetwork drop";
            mDeque.pop_back ();
        }

        auto jm = broadcast ? j_.debug() : j_.info();
        JLOG (jm) <<
            "RPCCall::fromNetwork push: " << jvObj;

        mDeque.push_back (std::make_pair (mSeq++, jvObj));

        if (!mSending)
        {
            JLOG (j_.info()) << "RPCCall::fromNetwork start";

            mSending = m_jobQueue.addJob (
                jtCLIENT, "RPCSub::sendThread", [this] (Job&) {
                    sendThread();
                });
        }
    }

    void setUsername (std::string const& strUsername) override
    {
        ScopedLockType sl (mLock);

        mUsername = strUsername;
    }

    void setPassword (std::string const& strPassword) override
    {
        ScopedLockType sl (mLock);

        mPassword = strPassword;
    }

private:
    void sendThread ()
    {
        Json::Value jvEvent;
        bool bSend;

        do
        {
            {
                ScopedLockType sl (mLock);

                if (mDeque.empty ())
                {
                    mSending    = false;
                    bSend       = false;
                }
                else
                {
                    std::pair<int, Json::Value> pEvent  = mDeque.front ();

                    mDeque.pop_front ();

                    jvEvent     = pEvent.second;
                    jvEvent["seq"]  = pEvent.first;

                    bSend       = true;
                }
            }

            if (bSend)
            {
                try
                {
                    JLOG (j_.info()) << "RPCCall::fromNetwork: " << mIp;

                    RPCCall::fromNetwork (
                        m_io_service,
                        mIp, mPort,
                        mUsername, mPassword,
                        mPath, "event",
                        jvEvent,
                        mSSL,
                        true,
                        logs_);
                }
                catch (const std::exception& e)
                {
                    JLOG (j_.info()) << "RPCCall::fromNetwork exception: " << e.what ();
                }
            }
        }
        while (bSend);
    }

private:
    enum
    {
        eventQueueMax = 32
    };

    boost::asio::io_service& m_io_service;
    JobQueue& m_jobQueue;

    std::string             mUrl;
    std::string             mIp;
    std::uint16_t           mPort;
    bool                    mSSL;
    std::string             mUsername;
    std::string             mPassword;
    std::string             mPath;

    int                     mSeq;                       

    bool                    mSending;                   

    std::deque<std::pair<int, Json::Value> >    mDeque;

    beast::Journal j_;
    Logs& logs_;
};


RPCSub::RPCSub (InfoSub::Source& source)
    : InfoSub (source, Consumer())
{
}

std::shared_ptr<RPCSub> make_RPCSub (
    InfoSub::Source& source, boost::asio::io_service& io_service,
    JobQueue& jobQueue, std::string const& strUrl,
    std::string const& strUsername, std::string const& strPassword,
    Logs& logs)
{
    return std::make_shared<RPCSubImp> (std::ref (source),
        std::ref (io_service), std::ref (jobQueue),
            strUrl, strUsername, strPassword, logs);
}

} 





