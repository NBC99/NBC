

#ifndef RIPPLE_TX_CANCELTICKET_H_INCLUDED
#define RIPPLE_TX_CANCELTICKET_H_INCLUDED

#include <ripple/app/tx/impl/Transactor.h>
#include <ripple/basics/Log.h>
#include <ripple/protocol/Indexes.h>

namespace ripple {

class CancelTicket
    : public Transactor
{
public:
    explicit CancelTicket (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }

    static
    NotTEC
    preflight (PreflightContext const& ctx);

    TER doApply () override;
};

}

#endif








