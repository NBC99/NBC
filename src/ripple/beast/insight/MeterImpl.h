

#ifndef BEAST_INSIGHT_METERIMPL_H_INCLUDED
#define BEAST_INSIGHT_METERIMPL_H_INCLUDED

#include <ripple/beast/insight/BaseImpl.h>

namespace beast {
namespace insight {

class Meter;

class MeterImpl
    : public std::enable_shared_from_this <MeterImpl>
    , public BaseImpl
{
public:
    using value_type = std::uint64_t;

    virtual ~MeterImpl () = 0;
    virtual void increment (value_type amount) = 0;
};

}
}

#endif








