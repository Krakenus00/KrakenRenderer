// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include <memory>
#include <random>
#include <vector>

#include "KrakenWindows.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "DX11APIMacro.h"
#include "IGraphicsCore.h"
#include "KrakenHelpers.h"

namespace KrakenGraphics
{
    class DIRECTX11CORE_API DirectX11Core: public IGraphicsCore
    {
	public:
		DirectX11Core();
		DirectX11Core(const DirectX11Core&) = delete;
		DirectX11Core& operator=(const DirectX11Core&) = delete;
		~DirectX11Core();

		void EndFrame();
		void BeginFrame(float red, float green, float blue) noexcept;
		void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DirectX::XMMATRIX GetProjection() const noexcept;
		void SetCamera(DirectX::FXMMATRIX cam) noexcept;
		DirectX::XMMATRIX GetCamera() const noexcept;
		void EnableImgui() noexcept;
		void DisableImgui() noexcept;
		bool IsImguiEnabled() const noexcept;
	private:
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX camera;
		bool imguiEnabled = true;
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
    };
}