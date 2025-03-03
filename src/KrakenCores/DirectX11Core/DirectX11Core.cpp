// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#include "DirectX11Core.h"
#include "DirectXHelpers.h"
#include "DirectXException.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

namespace KrakenGraphics
{
	DirectX11Core::DirectX11Core(HWND hWnd)
		: projection()
		, camera()
		, pDevice()
		, pSwap()
		, pContext()
		, pTarget()
		, pDSV()
	{
		DxBeginTrace();

		DXGI_SWAP_CHAIN_DESC sd
		{
			.BufferDesc
			{
				.Width            = 0,
				.Height           = 0,
				.RefreshRate
				{
					.Numerator    = 0,
					.Denominator  = 0
				},
				.Format           = DXGI_FORMAT_B8G8R8A8_UNORM,
				.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
				.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED
			},
			.SampleDesc
			{
				.Count            = 1,
				.Quality          = 0
			},
			.BufferUsage          = DXGI_USAGE_RENDER_TARGET_OUTPUT,
			.BufferCount          = 1,
			.OutputWindow         = hWnd,
			.Windowed             = TRUE,
			.SwapEffect           = DXGI_SWAP_EFFECT_DISCARD,
			.Flags                = 0
		};

		UINT swapCreateFlags = 0u;
	#ifndef NDEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		// Create device and front/back buffers, and swap chain and rendering context
		DxCheck(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		));

		// Gain access to texture subresource in swap chain (back buffer)
		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		DxCheck(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		DxCheck(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

		// Create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc
		{
			.DepthEnable    = TRUE,
			.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			.DepthFunc      = D3D11_COMPARISON_LESS
		};
		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		DxCheck(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

		// Bind depth state
		pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

		// Create depth stensil texture
		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth
		{
			.Width       = 800u,
			.Height      = 600u,
			.MipLevels   = 1u,
			.ArraySize   = 1u,
			.Format      = DXGI_FORMAT_D32_FLOAT,
			.SampleDesc
			{
				.Count   = 1u,
				.Quality = 0u
			},
			.Usage       = D3D11_USAGE_DEFAULT,
			.BindFlags   = D3D11_BIND_DEPTH_STENCIL
		};
		DxCheck(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// Create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV
		{
			.Format        = DXGI_FORMAT_D32_FLOAT,
			.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
			.Flags         = 0u,
			.Texture2D
			{
				.MipSlice  = 0u
			}
		};
		DxCheck(pDevice->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &pDSV
		));

		// bind depth stensil view to OM
		pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800.0f;
		vp.Height = 600.0f;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pContext->RSSetViewports(1u, &vp);
	}

	void DirectX11Core::EndFrame()
	{
		DxBeginTrace();

		if (FAILED(hr = pSwap->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw DxDeviceRemovedException(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				throw DxException(hr);
			}
		}
	}

	void DirectX11Core::BeginFrame(float red, float green, float blue) noexcept
	{
		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void DirectX11Core::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
	{
		DxBeginTrace();

		DxCheckInfo(pContext->DrawIndexed(count, 0u, 0u));
	}

	void DirectX11Core::SetProjection(DirectX::FXMMATRIX proj) noexcept
	{
		projection = proj;
	}

	DirectX::XMMATRIX DirectX11Core::GetProjection() const noexcept
	{
		return projection;
	}

	void DirectX11Core::SetCamera(DirectX::FXMMATRIX cam) noexcept
	{
		camera = cam;
	}

	DirectX::XMMATRIX DirectX11Core::GetCamera() const noexcept
	{
		return camera;
	}
}
