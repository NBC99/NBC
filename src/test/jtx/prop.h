

#ifndef RIPPLE_TEST_JTX_PROP_H_INCLUDED
#define RIPPLE_TEST_JTX_PROP_H_INCLUDED

#include <test/jtx/Env.h>
#include <memory>

namespace ripple {
namespace test {
namespace jtx {


template <class Prop>
struct prop
{
    std::unique_ptr<basic_prop> p_;

    template <class... Args>
    prop(Args&&... args)
        : p_(std::make_unique<
            prop_type<Prop>>(
                std::forward <Args> (
                    args)...))
    {
    }

    void
    operator()(Env& env, JTx& jt) const
    {
        jt.set(p_->clone());
    }
};

} 
} 
} 

#endif








