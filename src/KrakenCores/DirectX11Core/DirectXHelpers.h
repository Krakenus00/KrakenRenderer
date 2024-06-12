// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "DirectXException.h"

#ifndef NDEBUG
    #include "DXGIInfoManager.h"
#endif

// NOTE: HRESULT hr should exist in the local scope for these macros to work


// Set up everything required to trace Windows errors.
#define WinBeginTrace() \
    HRESULT hr = 0

// Helper macro to create an instance of KrakenGraphics::WindowsException from an HRESULT.
// In Debug mode, the macro also provides code location where it was invoked.
#define WinException(hr) \
    ::KrakenGraphics::WindowsException(hr, L"", {}, DEBUG_TRACE)

// Throw KrakenGraphics::WindowsException if hrcall returned a failure HRESULT.
#define WinCheck(hrcall) \
    do { \
        if (FAILED(hr = (hrcall))) \
            throw WinException(hr); \
    } while (0)

#ifndef NDEBUG
    // Set up everything required to trace DirectX errors.
    #define DxBeginTrace() \
        WinBeginTrace(); \
        auto& infoManager = ::KrakenGraphics::DXGIInfoManager::GetInstance()

    // Helper macro to create an instance of KrakenGraphics::DirectXException from an HRESULT.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    #define DxException(hr) \
        ::KrakenGraphics::DirectXException(hr, L"", infoManager.GetMessages(), DEBUG_TRACE)
    
    // Throw KrakenGraphics::DirectXException if hrcall returned a failure HRESULT.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    #define DxCheck(hrcall) \
        do { \
            infoManager.Set(); \
            if(FAILED(hr = (hrcall))) \
                throw DxException(hr); \
        } while(0)
    
    // Helper macro to create an instance of KrakenGraphics::DirectXDeviceRemovedException from an HRESULT.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    #define DxDeviceRemovedException(hr) \
        ::KrakenGraphics::DirectXDeviceRemovedException(hr, L"", infoManager.GetMessages(), DEBUG_TRACE)
    
    // Variation of DxCheck macro for functions that do not return HRESULT.
    // Throw KrakenGraphics::DirectXException if hrcall failed.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    // In Release mode, does nothing.
    #define DxCheckInfo(call) \
        do { \
            infoManager.Set(); \
            (call); \
            decltype(auto) msgs = infoManager.GetMessages(); \
            if (!msgs.empty()) \
                throw ::KrakenGraphics::DirectXException(hr, L"", msgs, DEBUG_TRACE); \
        } while (0)
#else
    // Set up everything required to trace DirectX errors.
    #define DxBeginTrace() WinBeginTrace()

    // Helper macro to create an instance of KrakenGraphics::DirectXException from an HRESULT.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    #define DxException(hr) \
        ::KrakenGraphics::DirectXException(hr)
    
    // Throw KrakenGraphics::DirectXException if hrcall returned a failure HRESULT.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    #define DxCheck(hrcall) \
        do { \
            if (FAILED(hr = (hrcall))) \
                throw DxException(hr); \
        } while(0)
    
    // Helper macro to create an instance of KrakenGraphics::DirectXDeviceRemovedException from an HRESULT.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    #define DxDeviceRemovedException(hr) \
        ::KrakenGraphics::DirectXDeviceRemovedException(hr)
    
    // Variation of DxCheck macro for functions that do not return HRESULT.
    // Throw KrakenGraphics::DirectXException if hrcall failed.
    // In Debug mode, the macro also provides code location where it was invoked and debug information from DXGI debugger.
    // In Release mode, does nothing.
    #define DxCheckInfo(call) (call)
#endif
