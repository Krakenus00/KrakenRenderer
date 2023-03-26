#pragma once
// HRESULT hr should exist in the local scope for these macros to work

#define WinBeginTrace() HRESULT hr
#define WinExcept(hr) KrakenGraphics::WindowsException(hr, L"", {} DEBUG_TRACE)
#define WinLast() WinExcept(GetLastError())
#define WinCheck(hrcall) do { if( FAILED( hr = (hrcall) ) ) throw WinExcept(hr); } while (0)

#ifndef NDEBUG
#define DxExcept(hr) WinExcept(hr)
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

// Call this macro in
#ifdef NDEBUG
#define DxBeginTrace(gfx) WinBeginTrace()
#else
#define DX_BEGIN_TRACE(gfx) WinBeginTrace(); KrakenGraphics::DXGIInfoManager& infoManager = GetInfoManager((gfx))
#endif
