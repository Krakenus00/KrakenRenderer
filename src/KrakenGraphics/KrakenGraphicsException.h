// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
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
