#pragma once

#include <exception>
#include <string>

#include "KrakenHelpers.h"

namespace KrakenGraphics
{
    class KrakenException
    {
    public: // Non-static constructors and desctructor
        KrakenException() = default;
        KrakenException(const KrakenException&) = default;
        KrakenException(KrakenException&&) = default;
        KrakenException(const std::wstring& description);
    #ifndef NDEBUG
        KrakenException(const std::wstring& description, size_t line, const std::wstring& file);
    #endif
        ~KrakenException() = default;

    public: // Non-static members
        virtual std::wstring what() const { return _description; }
    #ifndef NDEBUG
        std::wstring file() const { return _file; }
        size_t       line() const { return _line; }
    #endif

    private: // Non-static fields
        std::wstring    _description;
    #ifndef NDEBUG
        size_t          _line;
        std::wstring    _file;
    #endif
    };
}

// Helper to switch between Release and Debug donstructors
#ifdef NDEBUG
#define DEBUG_TRACE
#else
#define DEBUG_TRACE , __LINE__, __WFILE__
#endif
