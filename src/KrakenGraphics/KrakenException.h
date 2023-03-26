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
        KrakenException() noexcept = default;
        KrakenException(const KrakenException&) noexcept = default;
        KrakenException(KrakenException&&) noexcept = default;
        ~KrakenException() noexcept = default;

        KrakenException(const std::wstring& description, size_t line = 0, const std::wstring& file = L"") noexcept;

    public: // Operators
        KrakenException& operator=(const KrakenException&) = default;
        KrakenException& operator=(KrakenException&&) = default;

    public: // Members
        virtual const std::wstring& what() const noexcept { return _description; }
        const std::wstring& file() const noexcept { return _file; }
        size_t line() const noexcept { return _line; }

    protected: // Fields
        std::wstring    _description;
        size_t          _line;
        std::wstring    _file;
    };
}

#ifdef NDEBUG
#define DEBUG_TRACE
#else
#define DEBUG_TRACE , __LINE__, __WFILE__
#endif
