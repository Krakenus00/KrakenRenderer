// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "DirectX11CoreAPIMacro.h"

#include <memory>
#include <random>
#include <vector>

#include "KrakenWindows.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl.h>

#include "IGraphicsCore.h"
#include "KrakenHelpers.h"

namespace KrakenGraphics
{
    class DirectX11Core: public IGraphicsCore
    {
	public:
		DIRECTX11CORE_API DirectX11Core() = delete;
		DIRECTX11CORE_API DirectX11Core(HWND hwnd);
		DIRECTX11CORE_API DirectX11Core(const DirectX11Core&) = delete;
		DIRECTX11CORE_API DirectX11Core& operator=(const DirectX11Core&) = delete;
		DIRECTX11CORE_API ~DirectX11Core() = default;

		DIRECTX11CORE_API void EndFrame();
		DIRECTX11CORE_API void BeginFrame(float red, float green, float blue) noexcept;
		DIRECTX11CORE_API void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
		DIRECTX11CORE_API void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DIRECTX11CORE_API DirectX::XMMATRIX GetProjection() const noexcept;
		DIRECTX11CORE_API void SetCamera(DirectX::FXMMATRIX cam) noexcept;
		DIRECTX11CORE_API DirectX::XMMATRIX GetCamera() const noexcept;

	private:
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX camera;
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
    };
}