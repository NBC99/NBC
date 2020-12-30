

#ifndef RIPPLE_CORE_LOADMONITOR_H_INCLUDED
#define RIPPLE_CORE_LOADMONITOR_H_INCLUDED

#include <ripple/basics/UptimeClock.h>
#include <ripple/core/LoadEvent.h>
#include <ripple/beast/utility/Journal.h>
#include <chrono>
#include <mutex>

namespace ripple {


class LoadMonitor
{
public:
    explicit
    LoadMonitor (beast::Journal j);

    void addLoadSample (LoadEvent const& sample);

    void addSamples (int count, std::chrono::milliseconds latency);

    void setTargetLatency (std::chrono::milliseconds avg,
                           std::chrono::milliseconds pk);

    bool isOverTarget (std::chrono::milliseconds avg,
                       std::chrono::milliseconds peak);

    struct Stats
    {
        Stats();

        std::uint64_t             count;
        std::chrono::milliseconds latencyAvg;
        std::chrono::milliseconds latencyPeak;
        bool                      isOverloaded;
    };

    Stats getStats ();

    bool isOver ();

private:
    void update ();

    std::mutex mutex_;

    std::uint64_t             mCounts;
    int                       mLatencyEvents;
    std::chrono::milliseconds mLatencyMSAvg;
    std::chrono::milliseconds mLatencyMSPeak;
    std::chrono::milliseconds mTargetLatencyAvg;
    std::chrono::milliseconds mTargetLatencyPk;
    UptimeClock::time_point   mLastUpdate;
    beast::Journal j_;
};

} 

#endif








