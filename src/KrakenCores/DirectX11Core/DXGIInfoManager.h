// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include <string>
#include <vector>

#include "KrakenWindows.h"
#include <dxgidebug.h>
#include <wrl.h>

namespace KrakenGraphics
{
	class DXGIInfoManager
	{
	private:
		DXGIInfoManager();

	public:
		~DXGIInfoManager() = default;
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
		static DXGIInfoManager& GetInstance() noexcept;

		void Set() noexcept;
		std::vector<std::wstring> GetMessages() const;

	private:
		UINT64 next;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
	};
}
