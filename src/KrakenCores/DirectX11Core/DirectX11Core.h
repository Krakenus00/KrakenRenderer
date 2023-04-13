// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "DirectX11CoreAPIMacro.h"

#include "IGraphicsCore.h"
#include "KrakenHelpers.h"

#ifndef NDEBUG
#include "DXGIInfoManager.h"
#endif

#include <memory>
#include <random>
#include <vector>

#include "KrakenWindows.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl.h>

namespace KrakenGraphics
{
    class DIRECTX11CORE_API DirectX11Core: public IGraphicsCore
    {
	public:
		DirectX11Core() = delete;
		DirectX11Core(HWND hwnd);
		DirectX11Core(const DirectX11Core&) = delete;
		DirectX11Core& operator=(const DirectX11Core&) = delete;
		~DirectX11Core() = default;

		void EndFrame();
		void BeginFrame(float red, float green, float blue) noexcept;
		void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DirectX::XMMATRIX GetProjection() const noexcept;
		void SetCamera(DirectX::FXMMATRIX cam) noexcept;
		DirectX::XMMATRIX GetCamera() const noexcept;

	private:
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX camera;
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	#ifndef NDEBUG
		std::shared_ptr<DXGIInfoManager> pInfoManager;
	#endif
    };
}