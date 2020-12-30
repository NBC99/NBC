

#include <ripple/core/impl/Workers.h>
#include <ripple/beast/unit_test.h>
#include <ripple/basics/PerfLog.h>
#include <ripple/core/JobTypes.h>
#include <ripple/json/json_value.h>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>

namespace ripple {



namespace perf {

class PerfLogTest
    : public PerfLog
{
    void rpcStart(std::string const &method, std::uint64_t requestId) override
    {}

    void rpcFinish(std::string const &method, std::uint64_t requestId) override
    {}

    void rpcError(std::string const &method, std::uint64_t dur) override
    {}

    void jobQueue(JobType const type) override
    {}

    void jobStart(JobType const type,
        std::chrono::microseconds dur,
        std::chrono::time_point<std::chrono::steady_clock> startTime,
        int instance) override
    {}

    void jobFinish(JobType const type, std::chrono::microseconds dur,
        int instance) override
    {}

    Json::Value countersJson() const override
    {
        return Json::Value();
    }

    Json::Value currentJson() const override
    {
        return Json::Value();
    }

    void resizeJobs(int const resize) override
    {}

    void rotate() override
    {}
};

} 


class Workers_test : public beast::unit_test::suite
{
public:
    struct TestCallback : Workers::Callback
    {
        void processTask(int instance) override
        {
            std::lock_guard<std::mutex> lk{mut};
            if (--count == 0)
                cv.notify_all();
        }

        std::condition_variable cv;
        std::mutex              mut;
        int                     count = 0;
    };

    void testThreads(int const tc1, int const tc2, int const tc3)
    {
        testcase("threadCounts: " + std::to_string(tc1) +
            " -> " + std::to_string(tc2) + " -> " + std::to_string(tc3));

        TestCallback cb;
        std::unique_ptr<perf::PerfLog> perfLog =
            std::make_unique<perf::PerfLogTest>();

        Workers w(cb, *perfLog, "Test", tc1);
        BEAST_EXPECT(w.getNumberOfThreads() == tc1);

        auto testForThreadCount = [this, &cb, &w] (int const threadCount)
        {
            cb.count = threadCount;

            w.setNumberOfThreads(threadCount);
            BEAST_EXPECT(w.getNumberOfThreads() == threadCount);

            for (int i = 0; i < threadCount; ++i)
                w.addTask();

            using namespace std::chrono_literals;
            std::unique_lock<std::mutex> lk{cb.mut};
            bool const signaled = cb.cv.wait_for(lk, 10s, [&cb]{return cb.count == 0;});
            BEAST_EXPECT(signaled);
            BEAST_EXPECT(cb.count == 0);
        };
        testForThreadCount (tc1);
        testForThreadCount (tc2);
        testForThreadCount (tc3);
        w.pauseAllThreadsAndWait();

        BEAST_EXPECT(cb.count == 0);
    }

    void run() override
    {
        testThreads( 0, 0,  0);
        testThreads( 1, 0,  1);
        testThreads( 2, 1,  2);
        testThreads( 4, 3,  5);
        testThreads(16, 4, 15);
        testThreads(64, 3, 65);
    }
};

BEAST_DEFINE_TESTSUITE(Workers, core, ripple);

}






