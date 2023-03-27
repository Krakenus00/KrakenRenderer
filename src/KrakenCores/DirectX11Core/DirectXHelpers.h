// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

// HRESULT hr should exist in the local scope for these macros to work

#define WinBeginTrace() HRESULT hr
#define WinException(hr) KrakenGraphics::WindowsException(hr, L"", {}, DEBUG_TRACE)
#define WinLast() WinExcept(GetLastError())
#define WinCheck(hrcall) do { if( FAILED( hr = (hrcall) ) ) throw WinException(hr); } while (0)

#ifndef NDEBUG
#define DxException(hr) KrakenGraphics::DirectXException(hr, L"", infoManager.GetMessages(), DEBUG_TRACE)
#define DxCheck(hrcall) do { infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw DxException(hr); } while(0)
#define DxDeviceRemovedException(hr) KrakenGraphics::DirectXDeviceRemovedException(hr, L"", infoManager.GetMessages(), DEBUG_TRACE)
#define DxCheckInfo(call) do { infoManager.Set(); (call); {auto msgs = infoManager.GetMessages(); if(!msgs.empty()) throw KrakenGraphics::DirectXException(hr, L"", msgs, DEBUG_TRACE);}} while(0)
#else
#define DxException(hr) KrakenGraphics::DirectXException(hr)
#define DxCheck(hrcall) do { if( FAILED( hr = (hrcall) ) ) throw DxException(hr); } while(0)
#define DxDeviceRemovedException(hr) KrakenGraphics::DirectXDeviceRemovedException(hr)
#define DxCheckInfo(call) (call)
#endif

// Call this macro at the beginning of every DirectX-related function to support error-handling
#ifdef NDEBUG
#define DxBeginTrace(gfx) WinBeginTrace()
#else
#define DxBeginTrace(gfx) WinBeginTrace(); KrakenGraphics::DXGIInfoManager& infoManager = GetInfoManager((gfx))
#endif
