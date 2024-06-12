// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include <exception>
#include <string>

#include "KrakenHelpers.h"

namespace KrakenGraphics
{
    class KrakenException
    {
    public: // Constructors and desctructor
        KrakenException() noexcept;
        KrakenException(const std::wstring& description, size_t line = 0, const std::wstring& file = L"") noexcept;
        KrakenException(const KrakenException&) noexcept = default;
        KrakenException(KrakenException&&) noexcept = default;
        ~KrakenException() noexcept = default;

    public: // Operators
        KrakenException& operator=(const KrakenException&) = default;
        KrakenException& operator=(KrakenException&&) = default;

    public: // Members
        const std::wstring& description() const noexcept { return _description; }
        const std::wstring& file() const noexcept { return _file; }
        size_t line() const noexcept { return _line; }

        virtual std::wstring what() const noexcept { return description(); }

    protected: // Fields
        std::wstring    _description;
        size_t          _line;
        std::wstring    _file;
    };
}

#ifdef NDEBUG
// Helper macro to trace places in the source code. Release version does not provide any information.
#define DEBUG_TRACE 0
#else
// Helper macro to trace places in the source code. Debug version provides values for source file and code line.
#define DEBUG_TRACE __LINE__, __WFILE__
#endif
