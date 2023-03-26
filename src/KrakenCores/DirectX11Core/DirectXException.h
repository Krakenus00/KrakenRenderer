#pragma once

#include "WindowsException.h"
#include "GraphicsException.h"

namespace KrakenGraphics
{
    class DirectXException
        : public WindowsException
        , public GraphicsException
    {

    };
}
