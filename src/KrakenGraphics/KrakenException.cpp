// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#include "KrakenException.h"

#include "KrakenHelpers.h"

namespace KrakenGraphics
{
    KrakenException::KrakenException(const std::wstring& description)
        : _description(description)
    #ifndef NDEBUG
        , _line(0)
        , _file(L"")
    #endif
    {}

#ifndef NDEBUG
    KrakenException::KrakenException(const std::wstring& description, size_t line, const std::wstring& file)
        : _description(description)
        , _line(line)
        , _file(file)
    {}
#endif
}
