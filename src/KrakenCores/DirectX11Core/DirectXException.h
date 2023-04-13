// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "WindowsException.h"

namespace KrakenGraphics
{
    class DirectXException: public WindowsException
    {
        using WindowsException::WindowsException;
    };

    class DirectXDeviceRemovedException: public DirectXException
    {
        using DirectXException::DirectXException;
    };
}
