
#ifndef RIPPLE_TEST_CSF_TIMERS_H_INCLUDED
#define RIPPLE_TEST_CSF_TIMERS_H_INCLUDED

#include <chrono>
#include <ostream>
#include <test/csf/Scheduler.h>
#include <test/csf/SimTime.h>

namespace ripple {
namespace test {
namespace csf {




class HeartbeatTimer
{
    Scheduler & scheduler_;
    SimDuration interval_;
    std::ostream & out_;

    RealTime startRealTime_;
    SimTime startSimTime_;

public:
    HeartbeatTimer(
        Scheduler& sched,
        SimDuration interval = std::chrono::seconds {60},
        std::ostream& out = std::cerr)
        : scheduler_{sched}, interval_{interval}, out_{out},
          startRealTime_{RealClock::now()},
          startSimTime_{sched.now()}
    {
    }

    void
    start()
    {
        scheduler_.in(interval_, [this](){beat(scheduler_.now());});
    }

    void
    beat(SimTime when)
    {
        using namespace std::chrono;
        RealTime realTime = RealClock::now();
        SimTime simTime = when;

        RealDuration realDuration = realTime - startRealTime_;
        SimDuration simDuration = simTime - startSimTime_;
        out_ << "Heartbeat. Time Elapsed: {sim: "
             << duration_cast<seconds>(simDuration).count()
             << "s | real: "
             << duration_cast<seconds>(realDuration).count()
             << "s}\n" << std::flush;

        scheduler_.in(interval_, [this](){beat(scheduler_.now());});
    }
};

}  
}  
}  

#endif








