// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

// HRESULT hr should exist in the local scope for these macros to work

#define WinBeginTrace() HRESULT hr
#define WinException(hr) ::KrakenGraphics::WindowsException(hr, L"", {}, DEBUG_TRACE)
#define WinCheck(hrcall) do { if( FAILED( hr = (hrcall) ) ) throw WinException(hr); } while (0)

#ifndef NDEBUG
#define DxException(hr) ::KrakenGraphics::DirectXException(hr, L"", pInfoManager->GetMessages(), DEBUG_TRACE)
#define DxCheck(hrcall) do { pInfoManager->Set(); if( FAILED( hr = (hrcall) ) ) throw DxException(hr); } while(0)
#define DxDeviceRemovedException(hr) ::KrakenGraphics::DirectXDeviceRemovedException(hr, L"", pInfoManager->GetMessages(), DEBUG_TRACE)
#define DxCheckInfo(call) do { pInfoManager->Set(); (call); {auto msgs = pInfoManager->GetMessages(); if(!msgs.empty()) throw ::KrakenGraphics::DirectXException(hr, L"", msgs, DEBUG_TRACE);}} while (0)
#else
#define DxException(hr) ::KrakenGraphics::DirectXException(hr)
#define DxCheck(hrcall) do { if( FAILED( hr = (hrcall) ) ) throw DxException(hr); } while(0)
#define DxDeviceRemovedException(hr) ::KrakenGraphics::DirectXDeviceRemovedException(hr)
#define DxCheckInfo(call) (call)
#endif

// Call this macro at the beginning of every DirectX-related function to support error-handling
#ifdef NDEBUG
#define DxBeginTrace(gfx) WinBeginTrace()
#else
#define DxBeginTrace(gfx) do { WinBeginTrace(); std::shared_ptr<::KrakenGraphics::DXGIInfoManager> pInfoManager = GetInfoManager((gfx)); } while (0)
#endif
