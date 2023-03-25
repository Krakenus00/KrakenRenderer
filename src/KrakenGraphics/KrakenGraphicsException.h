#pragma once

#include "KrakenException.h"

namespace KrakenGraphics
{
    class KrakenGraphicsException: public KrakenException
    {
    public:
        using KrakenException::KrakenException;
    };
}
