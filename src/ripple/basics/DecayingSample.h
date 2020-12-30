

#ifndef RIPPLE_BASICS_DECAYINGSAMPLE_H_INCLUDED
#define RIPPLE_BASICS_DECAYINGSAMPLE_H_INCLUDED

#include <chrono>
#include <cmath>

namespace ripple {


template <int Window, typename Clock>
class DecayingSample
{
public:
    using value_type = typename Clock::duration::rep;
    using time_point = typename Clock::time_point;

    DecayingSample () = delete;

    
    explicit DecayingSample (time_point now)
        : m_value (value_type())
        , m_when (now)
    {
    }

    
    value_type add (value_type value, time_point now)
    {
        decay (now);
        m_value += value;
        return m_value / Window;
    }

    
    value_type value (time_point now)
    {
        decay (now);
        return m_value / Window;
    }

private:
    void decay (time_point now)
    {
        if (now == m_when)
            return;

        if (m_value != value_type())
        {
            std::size_t elapsed = std::chrono::duration_cast<
                std::chrono::seconds>(now - m_when).count();

            if (elapsed > 4 * Window)
            {
                m_value = value_type();
            }
            else
            {
                while (elapsed--)
                    m_value -= (m_value + Window - 1) / Window;
            }
        }

        m_when = now;
    }

    value_type m_value;

    time_point m_when;
};



template <int HalfLife, class Clock>
class DecayWindow
{
public:
    using time_point = typename Clock::time_point;

    explicit
    DecayWindow (time_point now)
        : value_(0)
        , when_(now)
    {
    }

    void
    add (double value, time_point now)
    {
        decay(now);
        value_ += value;
    }

    double
    value (time_point now)
    {
        decay(now);
        return value_ / HalfLife;
    }

private:
    static_assert(HalfLife > 0,
        "half life must be positive");

    void
    decay (time_point now)
    {
        if (now <= when_)
            return;
        using namespace std::chrono;
        auto const elapsed =
            duration<double>(now - when_).count();
        value_ *= std::pow(2.0, - elapsed / HalfLife);
        when_ = now;
    }

    double value_;
    time_point when_;
};

}

#endif








