

#include <test/jtx/sig.h>
#include <test/jtx/utility.h>

namespace ripple {
namespace test {
namespace jtx {

void
sig::operator()(Env&, JTx& jt) const
{
    if (! manual_)
        return;
    jt.fill_sig = false;
    if(account_)
    {
        auto const account = *account_;
        jt.signer = [account](Env&, JTx& jt)
        {
            jtx::sign(jt.jv, account);
        };
    }
}

} 
} 
} 






